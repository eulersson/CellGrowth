// Custom
#include "AutomataParticle.h"

//Native
#include <iostream>
#include <random>

//Qt
#include <QTime>

AutomataParticle::AutomataParticle():Particle()
{
  m_alive = true;
  m_time = QTime::currentTime();
  m_rad = m_size*6;
}

AutomataParticle::AutomataParticle(qreal _x,
                                   qreal _y,
                                   qreal _z): Particle(_x,_y,_z)
{
  m_alive = true;
  m_time = QTime::currentTime();
  m_rad = m_size*6;
}

AutomataParticle::AutomataParticle(qreal _x,
                                   qreal _y,
                                   qreal _z,
                                   std::vector<unsigned int> _connectedParticles):Particle(_x,_y,_z,_connectedParticles)
{
  m_alive = true;
  m_time = QTime::currentTime();
  m_rad = m_size*6;
}

void AutomataParticle::calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance, unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor, bool _particleDeath)
{
  if(m_time.elapsed() % 500 == 0)
  {
    std::random_device rd;
    std::mt19937_64 gen(rd());

    QVector3D pos;

    std::vector<unsigned int> newAutoParticles;
    newAutoParticles.push_back(m_ID);

    std::uniform_real_distribution<float> distributionX (-(m_rad), m_rad);
    std::uniform_real_distribution<float> distributionY (-(m_rad), m_rad);
    std::uniform_real_distribution<float> distributionZ (-(m_rad), m_rad);

    float x= distributionX(gen);
    float y= distributionY(gen);
    float z= distributionZ(gen);

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;

    _particleList.push_back(std::unique_ptr<AutomataParticle> (new AutomataParticle(x,y,z,newAutoParticles)));
  }

  particleRules(_particleList);
}

std::vector<unsigned int> AutomataParticle::getNeighbours(std::vector<std::unique_ptr<Particle> > &_particleList)
{
  QVector3D neighbourPos;
  QVector3D distance;
  std::vector<unsigned int> neighbours;

  for(unsigned int i=0; i<_particleList.size(); i++)
  {
    if(m_ID != _particleList[i]->getID())
    {
      neighbourPos = _particleList[i]->getPosition();
      distance = m_pos - neighbourPos;
      float length = distance.length();

      if(length <= m_size*4)
      {
         neighbours.push_back(_particleList[i]->getID());
      }
    }
  }
  return neighbours;
}

void AutomataParticle::particleRules(std::vector<std::unique_ptr<Particle> > &_particleList)
{
  std::vector<unsigned int> neighbours;
  neighbours = getNeighbours(_particleList);

  unsigned int neighbourCount = neighbours.size();
  unsigned int particleCount = _particleList.size();

  if(particleCount>1)
  {
    if(neighbourCount>3)
    {
      m_alive = false;
    }
  }
  else if (particleCount>15)
  {
    if(neighbourCount <2 || neighbourCount>3)
    {
      m_alive = false;
    }
  }
  else if(m_alive==false && neighbourCount==3)
  {
    m_alive = true;
  }
}
