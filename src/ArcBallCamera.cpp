#include "ArcBallCamera.h"

ArcBallCamera::ArcBallCamera(
      QVector3D _position)
    : m_front(QVector3D(0.0f, 0.0f, 1.0f))
    , m_worldUp(QVector3D(0.0f, 1.0f, 0.0f))
    , m_right(QVector3D(1.0f, 0.0f, 0.0f))
    , m_rotationPoint(0,0,0)
    , m_view()
{

  m_view.translate(_position);
}

QMatrix4x4 ArcBallCamera::getViewMatrix()
{
  return m_view;
}

QVector3D ArcBallCamera::getPosition()
{
  QVector4D position = QVector4D (m_view(0,3), m_view(1,3), m_view(2,3), 1);
  position=position*m_view;
  qDebug("%f, %f, %f", position.x(), position.y(), position.z());
  return QVector3D(position.x(), position.y(), position.z());
}

GLfloat ArcBallCamera::getPitch()
{
  return m_pitch;
}

void ArcBallCamera::setRotationPoint(QVector3D rp)
{
  m_rotationPoint=rp;
}

void ArcBallCamera::processKeyboard(ARCCamera_Movement _direction, GLfloat _deltaTime)
{
  QVector3D dir;
  if (_direction == ARCFORWARD) {
      dir= m_up;
  }
  if (_direction == ARCBACKWARD) {
      dir=-m_up;

  }
  if (_direction == ARCLEFT) {
      dir=-m_right;
  }
  if (_direction == ARCRIGHT) {
      dir= m_right;
  }

  QVector3D vel = m_movementSpeed * dir;
  m_view.translate(vel);

}


QQuaternion ArcBallCamera::create_from_angle(
    const double &_xx, const double &_yy,
    const double &_zz, const double &_a)
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


  // Translate rotation point to origin to rotate about arbitrary point.
  m_view.translate(-m_rotationPoint);

  QQuaternion rotq;
  rotq=create_from_angle(0,1,0,qDegreesToRadians(_xoffset));

  m_view.rotate(rotq);
  rotq=create_from_angle(m_right.x(), m_right.y(), m_right.z(),qDegreesToRadians(_yoffset));
  m_view.rotate(rotq);

  // Translate rotation point back.
  m_view.translate(m_rotationPoint);

  // Update camera orientation vectors
  m_right.setX(-m_view(0,0));
  m_right.setY(-m_view(0,1));
  m_right.setZ(-m_view(0,2));

  m_up.setX(-m_view(1,0));
  m_up.setY(-m_view(1,1));
  m_up.setZ(-m_view(1,2));

  m_front.setX(-m_view(2,0));
  m_front.setY(-m_view(2,1));
  m_front.setZ(-m_view(2,2));


//  qDebug("%f, %f, %f",position.x(), position.y(), position.z());
//  qDebug("%f, %f, %f", m_view(0,3), m_view(1,3), m_view(2,3));

}


void ArcBallCamera::refocus()
{
  // Played on f-press
  QVector3D front=QVector3D(0,0,1);

  QVector3D pos = getPosition();

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
  // Create quaternion using absolute angle.
  QQuaternion rotq=create_from_angle(rotAxis.x(), rotAxis.y(), rotAxis.z(), -rotAngle);

  qDebug()<<rotAngle;
  QMatrix3x3 rotmat=rotq.toRotationMatrix();

  m_view.setToIdentity();
  m_view.rotate(rotq);
  m_view.translate(pos);


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
  QVector3D currpos=getPosition();

  GLfloat offset=-(steps*.8);
  QVector3D dir=currpos-m_rotationPoint;
  dir.normalize();
  QVector3D velocity = m_scrollSpeed * offset * dir;
  move(velocity);
}

void ArcBallCamera::move(QVector3D velocity)
{
//  QMatrix4x4 tm;
//  tm=m_view;
//  tm.translate(velocity);

  float dist = getPosition().length();


  if(dist>2)
  {
    m_view.translate(velocity);

  }
}
