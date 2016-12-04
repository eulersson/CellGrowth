#include "LinkedParticle.h"

// Default constructor puts it at origin
LinkedParticle::LinkedParticle():
  m_ID(m_ID_counter++)
{
  m_pos = QVector3D(0, 0, 0);

}

// Specify the location where we want to place it
LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z)
: m_ID(m_ID_counter++)
{
  m_pos = QVector3D(_x, _y, _z);
}


LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z,std::vector<int> _linkedParticles)
: m_ID(m_ID_counter++)
{
  m_pos = QVector3D(_x, _y, _z);
  m_linkedParticles=_linkedParticles;
}

// After force calculations are done, we advect the position
void LinkedParticle::advance()
{
  m_pos += m_vel;
}

// All the force calculation should happen in here
void LinkedParticle::calculate()
{
  //m_vel = 0.0005 * m_pos.normalized();
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

//adds linkedParticles ID to the linked Particle List
void LinkedParticle::link(int _ID)
{
  m_linkedParticles.push_back(_ID);
}

//Iterates through linked P. list and erases the input ID
void LinkedParticle::deleteLinke(int _ID)
{
  for(int i=0; i<m_linkedParticles.size();i++)
  {
    if(m_linkedParticles[i]==_ID)
      m_linkedParticles.erase(m_linkedParticles.begin()+i);
  }
}

//returns ID
int LinkedParticle::getID()
{
 return m_ID;
}


void LinkedParticle::split(std::vector<LinkedParticle> _particleList)

{

}

int LinkedParticle::m_ID_counter(0);
