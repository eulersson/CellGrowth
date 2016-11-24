#include "LinkedParticle.h"

LinkedParticle::LinkedParticle()
{
  m_pos.setX(0.0);
  m_pos.setY(0.0);
  m_pos.setZ(0.0);
}

LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}

void LinkedParticle::setPos(qreal _x,qreal _y,qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}

void LinkedParticle::getPos(QVector3D* _vector)
{
  _vector->setX(m_pos.x());
  _vector->setY(m_pos.y());
  _vector->setZ(m_pos.z());
}
