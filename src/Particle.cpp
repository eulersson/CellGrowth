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
    , m_foodLevel(false)
    , m_size(2.0)
    , m_foodThreshold(0)
{
  qDebug("Particle default constructor.");
}


Particle::Particle(qreal _x, qreal _y, qreal _z, float _size)
    : m_pos(QVector3D(_x, _y, _z))
    , m_ID(m_ID_counter++)
    , m_foodLevel(false)
    , m_size(_size)
    , m_foodThreshold(100)
{
  qDebug("Particle constructor passing in positions: %f,%f,%f", _x, _y, _z);
}


Particle::Particle(qreal _x,
      qreal _y,
      qreal _z,
      std::vector<unsigned int> _connectedParticles, float _size)
    : m_pos(QVector3D(_x, _y, _z))
    , m_ID(m_ID_counter++)
    , m_foodLevel(false)
    , m_size(_size)
    , m_foodThreshold(100)
{
 qDebug("Particle constructor passing in positions: %f,%f,%f and a list of"
         "particles", _x, _y, _z);
  m_connectedParticles = _connectedParticles;
}

void Particle::advance()
{
  m_pos += m_vel;
}

//void Particle::calculate(
//    std::vector<std::unique_ptr<Particle>> &_particleList,
//    QVector3D _averageDistance,
//    uint _particleCount,
//    QVector3D _lightPos,
//    int _cohesionFactor,
//    int _localCohesionFactor,
//    bool _particleDeath,
//    int _automataRadius,
//    int _automataTime)
//{

//}

void Particle::calculate(std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance, int _cohesionFactor, int _localCohesionFactor, bool _particleDeath)
{

}


void Particle::calculate(std::vector<std::unique_ptr<Particle> > &_particleList, int _automataRadius, int _automataTime)
{

}

void Particle::setFoodLevelTrue()
{
  m_foodLevel = true;
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

void Particle::connect(unsigned int _ID, std::vector<std::unique_ptr<Particle> > &_particleList)
{
  m_connectedParticles.push_back(_ID);
}

void Particle::deleteConnection(unsigned int _ID)
{
  for(size_t i=0;i<m_connectedParticles.size();i++)
  {
    if(m_connectedParticles[i]==_ID)
    {
      m_connectedParticles.erase(m_connectedParticles.begin() +i);
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

  for(size_t i=0;i<m_connectedParticles.size();i++)
  {
    _particleList[m_connectedParticles[i]]->getPos(tempVec);
    _linkPos.push_back(tempVec);
  }
}

void Particle::resetIDCounter()
{
  m_ID_counter=0;
}


