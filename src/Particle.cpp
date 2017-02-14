#include "Particle.h"
#include <iostream>
#include <random>


Particle::Particle()
  : m_ID(m_ID_counter++)
  , m_foodLevel(0)
  , m_foodTreshold(0)
  , m_pos(QVector3D(0,0,0))
  , m_size(0.1)
{
  std::cout << "Calling Particle Default Constructor" << std::endl;
}



Particle::Particle(qreal _x,qreal _y,qreal _z)
  : m_ID(m_ID_counter++)
  , m_foodLevel(0)
  , m_foodTreshold(100)
  , m_pos(QVector3D(_x, _y, _z))
  , m_size(0.1)
{
  std::cout
      << "Calling Particle Constructor with positions "
      << _x << ", " << _y << ", " << _z
      << std::endl;
}


Particle::Particle(qreal _x,qreal _y,qreal _z,std::vector<int> _connectedParticles)
  : m_ID(m_ID_counter++)
  , m_foodLevel(0)
  , m_foodTreshold(100)
  , m_pos(QVector3D(_x, _y, _z))
  , m_size(0.1)
{

  std::cout << "Calling Particle Custom Constructor" << std::endl;
  m_connectedParticles = _connectedParticles;

}



void Particle::advance()
{
  m_pos += m_vel;
}


void Particle::split()
{

}

void Particle::testForSplit()
{
  if (m_foodLevel >= m_foodTreshold)
  {
    m_split=true;
  }
  else
  {
    m_split=false;
  }
}

void Particle::setPos(qreal _x, qreal _y, qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}


void Particle::getPos(QVector3D &_pos)
{
  _pos.setX(m_pos.x());
  _pos.setY(m_pos.y());
  _pos.setZ(m_pos.z());
}


void Particle::connect(int _ID)
{
  m_connectedParticles.push_back(_ID);
}


void Particle::deleteConnection(int _ID)
{
  for (unsigned int i = 0; i < m_connectedParticles.size(); i++)
  {
    if (m_connectedParticles[i] == _ID)
    {
      m_connectedParticles.erase(m_connectedParticles.begin() + i);
    }
    break;
  }

}


int Particle::getID()
{

 return m_ID;
}


void Particle::getConnectionsID(std::vector<int> &_returnList)
{
  _returnList=m_connectedParticles;
}



int Particle::getConnectionCount()
{
  return m_connectedParticles.size();
}


void Particle::getPosFromConnections(std::vector<QVector3D> &_linkPos,std::vector<std::unique_ptr<Particle>> &_particleList)
{
  //looks for the Id in the particleList of the particle system and than gets the position

  _linkPos.clear();
  QVector3D tempVec;

  for(unsigned int i =0; i<m_connectedParticles.size();i++)
  {
    for(unsigned int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==m_connectedParticles[i])
      {
        _particleList[j]->getPos(tempVec);
        _linkPos.push_back(tempVec);
        break;

      }
    }
  }
}


// Initializes the counter (static member) to zero
int Particle::m_ID_counter(0);


int Particle::getPosInPS(std::vector<std::unique_ptr<Particle>> &_particleList)
{
  for (unsigned int i=0; i < _particleList.size(); i++)
  {
    if (_particleList[i]->getID() == m_ID)
    {
      return i;
    }
  }
}
s
