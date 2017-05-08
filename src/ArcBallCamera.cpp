#include "ArcBallCamera.h"

ArcBallCamera::ArcBallCamera(
      QVector3D _position,
      QVector3D _dist,
      QVector3D _up,
      GLfloat _yaw,
      GLfloat _pitch)
    : m_front(QVector3D(0.0f, 0.0f, 1.0f))
    , m_worldUp(QVector3D(0.0f, 1.0f, 0.0f))
    , m_right(QVector3D(1.0f, 0.0f, 0.0f))
    , m_movementSpeed(ARCSPEED)
    , m_mouseSensitivity(ARCSENSITIVTY)
    , m_rotationPoint(0,0,0)
    , m_view()
{
  this->m_position = QVector4D(_position.x(), _position.y(), _position.z(), 1);
//  this->m_worldUp = _up;
  this->m_yaw = _yaw;
  this->m_pitch = _pitch;
  this->m_dir = _dist;

  m_view(0,3)=m_position.x();
  m_view(1,3)=m_position.y();
  m_view(2,3)=m_position.z();
}

QMatrix4x4 ArcBallCamera::getViewMatrix()
{
  return m_view;
}

QVector3D ArcBallCamera::getPosition()
{
  return QVector3D(m_position.x(), m_position.y(), m_position.z());
}

GLfloat ArcBallCamera::getPitch()
{
  return this->m_pitch;
}

void ArcBallCamera::setRotationPoint(QVector3D rp)
{
  m_rotationPoint=rp;
}

void ArcBallCamera::processKeyboard(ARCCamera_Movement _direction, GLfloat _deltaTime)
{
  GLfloat velocity = this->m_movementSpeed * _deltaTime;
  if (_direction == ARCFORWARD) {
      m_dir=this->m_up;
  }
  if (_direction == ARCBACKWARD) {
      //SetDistance(velocity);
      m_dir=-this->m_up;

  }
  if (_direction == ARCLEFT) {
      m_dir=-this->m_right;
  }
  if (_direction == ARCRIGHT) {
      m_dir=this->m_right;
  }

  //Position+=this->Dir*1;
  QVector3D vel = m_dir*0.1;
  m_view.translate(vel);

  m_position=QVector3D(m_view(0,3), m_view(1,3), m_view(2,3));
  m_position=m_position*m_view;
}


QQuaternion ArcBallCamera::create_from_angle(const double &_xx, const double &_yy, const double &_zz, const double &_a)
{
    // Here we calculate the sin( theta / 2) once for optimization
    double factor = sin( _a * .5 );

    // Calculate the x, y and z of the quaternion
    double x = _xx * factor;
    double y = _yy * factor;
    double z = _zz * factor;

    // Calcualte the w value by cos( theta / 2 )
    double w = cos( _a * .5 );

    QQuaternion ret = QQuaternion(w, x, y, z);
    ret.normalize();
    return ret;
}


void ArcBallCamera::processMouseMovement(GLfloat _xoffset, GLfloat _yoffset)
{
  _xoffset =  _xoffset*m_mouseSensitivity;
  _yoffset =  _yoffset*m_mouseSensitivity;

  QQuaternion rotq;
  rotq=create_from_angle(0,1,0,qDegreesToRadians(_xoffset));
  m_quat=m_quat*rotq;

  rotq=create_from_angle(m_right.x(), m_right.y(), m_right.z(),qDegreesToRadians(_yoffset));
  m_quat=m_quat*rotq;


  QMatrix3x3 rotmat=m_quat.toRotationMatrix();
  m_view(0,0)=rotmat(0,0);
  m_view(0,1)=rotmat(0,1);
  m_view(0,2)=rotmat(0,2);

  m_view(1,0)=rotmat(1,0);
  m_view(1,1)=rotmat(1,1);
  m_view(1,2)=rotmat(1,2);

  m_view(2,0)=rotmat(2,0);
  m_view(2,1)=rotmat(2,1);
  m_view(2,2)=rotmat(2,2);

  m_position=QVector4D(m_view(0,3), m_view(1,3), m_view(2,3), 1);
  m_position=m_position*m_view;

  // Update camera orientation vectors
  m_right.setX(-rotmat(0,0));
  m_right.setY(-rotmat(0,1));
  m_right.setZ(-rotmat(0,2));

  m_up.setX(-rotmat(1,0));
  m_up.setY(-rotmat(1,1));
  m_up.setZ(-rotmat(1,2));

  m_front.setX(-rotmat(2,0));
  m_front.setY(-rotmat(2,1));
  m_front.setZ(-rotmat(2,2));

}


void ArcBallCamera::refocus()
{
  // Played on f-press
  QVector3D front=QVector3D(0,0,1);

  QVector3D pos = QVector3D(m_position.x(), m_position.y(), m_position.z());

  QVector3D forwardVector = m_rotationPoint-pos;
  forwardVector.normalize();

  float dot = QVector3D::dotProduct(front,forwardVector);

  if ( dot > 0.99999f)
  {
      // Vectors point in same direction, rotation point is already framed.
      return;
  }

  // Get the absolute rotation angle
  double rotAngle = acos(dot);
  // Get the axis we need to rotate around
  QVector3D rotAxis = QVector3D::crossProduct(front,forwardVector);
  rotAxis.normalize();
  // Create quaternion using absolute angle. This is a normal look at, so no rotation is being done.
  m_quat=create_from_angle(rotAxis.x(), rotAxis.y(), rotAxis.z(), -rotAngle);


  QMatrix3x3 rotmat=m_quat.toRotationMatrix();
  //rotmat = rotmat.transposed();

  m_view(0,0)=rotmat(0,0);
  m_view(0,1)=rotmat(0,1);
  m_view(0,2)=rotmat(0,2);

  m_view(1,0)=rotmat(1,0);
  m_view(1,1)=rotmat(1,1);
  m_view(1,2)=rotmat(1,2);

  m_view(2,0)=rotmat(2,0);
  m_view(2,1)=rotmat(2,1);
  m_view(2,2)=rotmat(2,2);

  m_view(0,3)=0;
  m_view(1,3)=0;
  m_view(2,3)=0;
  m_view.translate(QVector3D(m_position.x(), m_position.y(), m_position.z()));

  // Update camera orientation vectors
  m_right.setX(-rotmat(0,0));
  m_right.setY(-rotmat(0,1));
  m_right.setZ(-rotmat(0,2));

  m_up.setX(-rotmat(1,0));
  m_up.setY(-rotmat(1,1));
  m_up.setZ(-rotmat(1,2));

  m_front.setX(rotmat(2,0));
  m_front.setY(rotmat(2,1));
  m_front.setZ(rotmat(2,2));

}

void ArcBallCamera::processMouseScroll(int steps)
{
  GLfloat deltaTime=1.0f;
  GLfloat offset=-(steps*.8);
  QVector3D velocity = this->m_movementSpeed * offset * deltaTime * (QVector3D(m_position.x(), m_position.y(), m_position.z())
                                                                   -m_rotationPoint);
  move(velocity);
}

void ArcBallCamera::move(QVector3D velocity)
{
  QMatrix4x4 tm;
  tm=m_view;
  tm.translate(velocity);

  m_position=QVector4D(tm(0,3), tm(1,3), tm(2,3), 1);
  m_position=m_position*m_view;


  QVector3D dir = QVector3D(m_position.x(), m_position.y(), m_position.z())+m_rotationPoint;
  float dist = dir.length();


  if(dist>2)
  {
    m_view.translate(velocity);

  }
}
