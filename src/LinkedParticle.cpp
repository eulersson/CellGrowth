#include "LinkedParticle.h"

LinkedParticle::LinkedParticle()
{
  m_pos = QVector3D(0, 0, 0);
}

LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z)
{
  m_pos = QVector3D(_x, _y, _z);
}

void LinkedParticle::advance()
{
  m_pos += m_vel;
}

void LinkedParticle::calculate()
{
  m_vel = 0.001 * m_pos.normalized();
}

void LinkedParticle::setPos(qreal _x,qreal _y,qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}

void LinkedParticle::getPos(QVector3D &_vector)
{
  _vector.setX(m_pos.x());
  _vector.setY(m_pos.y());
  _vector.setZ(m_pos.z());
}
