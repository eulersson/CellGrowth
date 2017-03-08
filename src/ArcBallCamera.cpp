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

void ArcBallCamera::processMouseMovement(GLfloat _xoffset, GLfloat _yoffset, GLboolean _constrainPitch)
{
  _xoffset *= this->m_mouseSensitivity;
  _yoffset *= this->m_mouseSensitivity;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  //    if (constrainPitch)
  //    {
  //        if (this->Pitch+yoffset > 89.8f)
  //            yoffset=0;//this->Pitch = 89.0f;
  //        if (this->Pitch+yoffset < -89.8f)
  //            yoffset=0;//this->Pitch = -89.0f;
  //    }

  this->m_yaw   += _xoffset;
  this->m_pitch += _yoffset;

  //    QVector3D tmppos=Position;
  //    Position = QVector3D (
  //          cos(qDegreesToRadians(this->Yaw))* cos(qDegreesToRadians(this->Pitch)),
  //         -sin(qDegreesToRadians(this->Pitch)),
  //          sin(qDegreesToRadians(this->Yaw))* cos(qDegreesToRadians(this->Pitch))
  //    );

  //      Position = QVector3D (
  //            (cos((this->Yaw))    *tmppos.x()) - (sin((this->Yaw))  *tmppos.z()),// + (sin((this->Pitch))*tmppos.y()),
  //            (-sin((this->Pitch)) *tmppos.y() - cos((this->Pitch)) *tmppos.z()),
  //            (sin((this->Yaw))    *tmppos.x()) + (cos((this->Yaw))  *tmppos.z())// + (sin((this->Pitch))*tmppos.y())
  //                          );

  //      Position = QVector3D (
  //            Position.x()*tmppos.x(),
  //            Position.y(),
  //            Position.z()*tmppos.z()
  //                          );

  //      //Position+=tmppos;

  //    // Also re-calculate the Right and Up vector
  //    QVector3D right   = QVector3D::crossProduct(this->Front,this->WorldUp);
  //    QVector3D up      = QVector3D::crossProduct(this->Right,this->Front);
  //    this->Right       = right.normalized();
  //    this->Up          = up.normalized();

  //    view.lookAt(this->Position, rotationPoint , this->Up);


  // Also re-calculate the Right and Up vector
  //    QVector3D right   = QVector3D::crossProduct(this->Front,this->WorldUp);
  //    QVector3D up      = QVector3D::crossProduct(this->Right,this->Front);
  //    this->Right       = right.normalized();
  //    this->Up          = up.normalized();

  this->m_position=QVector3D(m_view(0,3), m_view(1,3), m_view(2,3));

  //Position=QVector3D(1,1,1);

  // Do x rotation.
  m_view.rotate(_xoffset, 0,1,0);
  // Get updated position by multiplying with the view matrix.
  m_position=m_position*m_view;
  QVector3D dir=(QVector3D(m_position.x(), m_position.y(), m_position.z())-m_rotationPoint);
  // Calculate new camera right.
  this->m_right=-QVector3D::crossProduct(dir,this->m_worldUp);
  this->m_right.normalize();
  // Do y rotation.
  m_view.rotate(_yoffset, this->m_right);
  //Position=Position*view;
  dir=(QVector3D(m_position.x(), m_position.y(), m_position.z())-m_rotationPoint);
  // Calculate camera up.
  this->m_up=-QVector3D::crossProduct(dir,this->m_right);
  this->m_up.normalize();

  //qDebug()<<QString::number(Position.z());
  //qDebug()<<QString::number(view(2,3));
  //qDebug()<<QString::number(Position.y());
  //qDebug()<<QString::number(Position.z());

}


void ArcBallCamera::refocus()
{
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

  this->m_position=QVector3D(tm(0,3), tm(1,3), tm(2,3));
  m_position=m_position*m_view;


  QVector3D dir = QVector3D(m_position.x(), m_position.y(), m_position.z())+m_rotationPoint;
  float dist = dir.length();


  if(dist>2)
  {
    m_view.translate(velocity);

  }
}
