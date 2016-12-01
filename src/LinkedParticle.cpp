#include "LinkedParticle.h"

// Default constructor puts it at origin
LinkedParticle::LinkedParticle()
{
  m_pos = QVector3D(0, 0, 0);
}

// Specify the location where we want to place it
LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z)
{
  m_pos = QVector3D(_x, _y, _z);
}

// After force calculations are done, we advect the position
void LinkedParticle::advance()
{
  m_pos += m_vel;
}

// All the force calculation should happen in here
void LinkedParticle::calculate()
{
  m_vel = 0.0005 * m_pos.normalized();
}

// For modifying the position of the particle
void LinkedParticle::setPos(qreal _x,qreal _y,qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}

// Position getter
void LinkedParticle::getPos(QVector3D &_vector)
{
  _vector.setX(m_pos.x());
  _vector.setY(m_pos.y());
  _vector.setZ(m_pos.z());
}
