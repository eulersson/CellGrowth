////////////////////////////////////////////////////////////////////////////////
/// @file Particle.cpp
/// @author Carola Gille
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Native
#include <iostream>
#include <random>

// Project
#include "Particle.h"


// Initialize static member
unsigned int Particle::m_ID_counter(0);


Particle::Particle()
    : m_pos(QVector3D(0,0,0))
    , m_ID(m_ID_counter++)
    , m_size(2.0)
    , m_foodLevel(0)
    , m_foodTreshold(0)
{
  qDebug("Particle default constructor.");
}


Particle::Particle(qreal _x, qreal _y, qreal _z)
    : m_pos(QVector3D(_x, _y, _z))
    , m_ID(m_ID_counter++)
    , m_size(2.0)
    , m_foodLevel(0)
    , m_foodTreshold(100)
{
  qDebug("Particle constructor passing in positions: %f,%f,%f", _x, _y, _z);
}


Particle::Particle(qreal _x,
    qreal _y,
    qreal _z,
    std::vector<unsigned int> _connectedParticles)
    : m_pos(QVector3D(_x, _y, _z))
    , m_ID(m_ID_counter++)
    , m_size(2.0)
    , m_foodLevel(0)
    , m_foodTreshold(100)
{
  qDebug("Particle constructor passing in positions: %f,%f,%f and a list of"
         "particles", _x, _y, _z);
  m_connectedParticles = _connectedParticles;
}

void Particle::advance()
{
  m_pos += m_vel;
}

void Particle::testForSplit()
{
  if (m_foodLevel >= m_foodTreshold)
    m_split=true;
  else
    m_split=false;
}


void Particle::getPos(QVector3D &_pos)
{
  _pos.setX(m_pos.x());
  _pos.setY(m_pos.y());
  _pos.setZ(m_pos.z());
}


void Particle::setPos(qreal _x, qreal _y, qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}

void Particle::getRadius(float &_radius)
{
  _radius = m_size;
}

void Particle::setRadius(float _radius)
{
  m_size = _radius;
}

void Particle::connect(unsigned int _ID)
{
  m_connectedParticles.push_back(_ID);
}


void Particle::deleteConnection(unsigned int _ID)
{
  for (size_t i = 0; i < m_connectedParticles.size(); i++)
  {
    if (m_connectedParticles[i] == _ID)
    {
      m_connectedParticles.erase(m_connectedParticles.begin() + i);
      break;
    }
  }
}


unsigned int Particle::getID()
{
 return m_ID;
}


void Particle::getConnectionsID(std::vector<unsigned int> &_returnList)
{
  _returnList=m_connectedParticles;
}


int Particle::getConnectionCount()
{
  return m_connectedParticles.size();
}


void Particle::getPosFromConnections(std::vector<QVector3D> &_linkPos,std::vector<std::unique_ptr<Particle>> &_particleList)
{
  // Looks for the Id in the particleList of the particle system and then gets the position
  _linkPos.clear();
  QVector3D tempVec;

  for(size_t i = 0; i < m_connectedParticles.size(); i++)
  {
    for(size_t j = 0; j < _particleList.size(); j++)
    {
      if(_particleList[j]->getID() == m_connectedParticles[i])
      {
        _particleList[j]->getPos(tempVec);
        _linkPos.push_back(tempVec);
        break;
      }
    }
  }
}


int Particle::getPosInPS(std::vector<std::unique_ptr<Particle>> &_particleList)
{
  for (unsigned int i = 0; i < _particleList.size(); i++)
  {
    if (_particleList[i]->getID() == m_ID)
      return i;
  }
  return -1; // Return negative one if none is found.
}
