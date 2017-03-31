#include "ArcBallCamera.h"

ArcBallCamera::ArcBallCamera(
      QVector3D _position,
      QVector3D _dist,
      QVector3D _up,
      GLfloat _yaw,
      GLfloat _pitch)
    : m_front(QVector3D(0.0f, 0.0f, -1.0f))
    , m_movementSpeed(ARCSPEED)
    , m_mouseSensitivity(ARCSENSITIVTY)
    , m_rotationPoint(0,0,0)
{
  this->m_position = QVector4D(_position.x(), _position.y(), _position.z(), 1);
  this->m_worldUp = _up;
  this->m_yaw = _yaw;
  this->m_pitch = _pitch;
  this->m_dir = _dist;

  m_view.setToIdentity();
  m_view.translate(this->m_dir*20);
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
      this->m_dir=this->m_up;
  }
  if (_direction == ARCBACKWARD) {
      //SetDistance(velocity);
      this->m_dir=-this->m_up;

  }
  if (_direction == ARCLEFT) {
      this->m_dir=-this->m_right;
  }
  if (_direction == ARCRIGHT) {
      this->m_dir=this->m_right;
  }

  //Position+=this->Dir*1;
  QVector3D vel = this->m_dir*0.1;
  m_view.translate(vel);

  this->m_position=QVector3D(m_view(0,3), m_view(1,3), m_view(2,3));
}



QQuaternion ArcBallCamera::create_from_angle(const double &xx, const double &yy, const double &zz, const double &a)
{
    // Here we calculate the sin( theta / 2) once for optimization
    double factor = sin( a / 2.0 );

    // Calculate the x, y and z of the quaternion
    double x = xx * factor;
    double y = yy * factor;
    double z = zz * factor;

    // Calcualte the w value by cos( theta / 2 )
    double w = cos( a / 2.0 );

    QQuaternion ret = QQuaternion(w, x, y, z);
    ret.normalize();
    return ret;
}


void ArcBallCamera::processMouseMovement(GLfloat _xoffset, GLfloat _yoffset, GLboolean _constrainPitch)
{
  _xoffset =  _xoffset*this->m_mouseSensitivity;
  _yoffset =  _yoffset*this->m_mouseSensitivity;

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

  m_position=QVector3D(m_view(0,3), m_view(1,3), m_view(2,3));
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
  // DOESNT WORK.
  m_view.translate(-QVector3D(m_position.x(), m_position.y(), m_position.z()));

  QVector3D dir=(QVector3D(m_position.x(), m_position.y(), m_position.z())-m_rotationPoint);
  dir.normalize();
  m_view(2,0)=-dir.x();
  m_view(2,1)=-dir.y();
  m_view(2,2)=-dir.z();

//    view(1,0)=Up.x();
//    view(1,1)=Up.y();
//    view(1,2)=Up.z();

  this->m_right=QVector3D::crossProduct(this->m_up,dir);
  m_view(0,0)=this->m_right.x();
  m_view(0,1)=this->m_right.y();
  m_view(0,2)=this->m_right.z();

  m_view.translate(QVector3D(m_position.x(), m_position.y(), m_position.z()));

//    view.setToIdentity();
//    view.lookAt(QVector3D(Position.x(), Position.y(), Position.z()), rotationPoint , this->WorldUp);
  //    qDebug()<<QString::number(view(2,3));
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

  m_position=QVector3D(tm(0,3), tm(1,3), tm(2,3));
  m_position=m_position*m_view;


  QVector3D dir = QVector3D(m_position.x(), m_position.y(), m_position.z())+m_rotationPoint;
  float dist = dir.length();


  if(dist>2)
  {
    m_view.translate(velocity);

  }
}
