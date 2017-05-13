////////////////////////////////////////////////////////////////////////////////
/// @file AutomataParticle.cpp
/// @author Lydia Kenton
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

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
}

AutomataParticle::AutomataParticle(qreal _x,
                                   qreal _y,
                                   qreal _z): Particle(_x,_y,_z, 2.0)
{
  m_alive = true;
  m_time = QTime::currentTime();
}

AutomataParticle::AutomataParticle(qreal _x,
                                   qreal _y,
                                   qreal _z,
                                   std::vector<unsigned int> _connectedParticles):Particle(_x,_y,_z,_connectedParticles, 2.0)
{
  m_alive = true;
  m_time = QTime::currentTime();
}

void AutomataParticle::calculate(std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance,
                                 unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor,
                                 bool _particleDeath, int _automataRadius, int _automataTime)
{
  //Generates a new particle at every time interval
  if(m_time.elapsed() % _automataTime == 0)
  {
    //Particles are randomly created on the screen within a set radius
    std::random_device rd;
    std::mt19937_64 gen(rd());

    QVector3D pos;

    std::vector<unsigned int> newAutoParticles;
    newAutoParticles.push_back(m_ID);

    int rad = m_size*_automataRadius;

    std::uniform_real_distribution<float> distributionX (-(rad), rad);
    std::uniform_real_distribution<float> distributionY (-(rad), rad);
    std::uniform_real_distribution<float> distributionZ (-(rad), rad);

    float x= distributionX(gen);
    float y= distributionY(gen);
    float z= distributionZ(gen);

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;

    //Adds a particle to the particle system
    _particleList.push_back(std::unique_ptr<AutomataParticle> (new AutomataParticle(x,y,z,newAutoParticles)));
  }

  //Function call to particleRules
  particleRules(_particleList);
}

std::vector<unsigned int> AutomataParticle::getNeighbours(std::vector<std::unique_ptr<Particle> > &_particleList)
{
  //Finds the number of neighbours for the current particle
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
  //Returns a vector of neighbour IDs
  return neighbours;
}

void AutomataParticle::particleRules(std::vector<std::unique_ptr<Particle> > &_particleList)
{
  std::vector<unsigned int> neighbours;
  //Function call to getNeighbours
  neighbours = getNeighbours(_particleList);

  unsigned int neighbourCount = neighbours.size();
  unsigned int particleCount = _particleList.size();

  //Rules to imitate Conway's Game of Life algorithm

  //Applies before a certain time as to avoid beginning the algorithm with less than three cells
  if(m_time.elapsed() <= 3000)
  {
    if(neighbourCount>3)
    {
      m_alive = false;
    }
  }
  else if(particleCount>10)
  {
    if(neighbourCount <2 || neighbourCount>3)
    {
      m_alive = false;
    }
  }
}
