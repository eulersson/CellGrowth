#include "AutomataParticle.h"
#include <iostream>
#include <random>

AutomataParticle::AutomataParticle():Particle()
{
  m_tick = 0;
  m_alive = true;
}

AutomataParticle::AutomataParticle(qreal _x,
                                   qreal _y,
                                   qreal _z): Particle(_x,_y,_z)
{
  m_tick = 0;
  m_alive = true;
}

AutomataParticle::AutomataParticle(qreal _x,
                                   qreal _y,
                                   qreal _z,
                                   std::vector<unsigned int> _connectedParticles):Particle(_x,_y,_z,_connectedParticles)
{
  m_tick = 0;
  m_alive = true;
}

void AutomataParticle::calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance, unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor, bool _particleDeath)
{
  if(m_tick % 300 == 0)
  {
  std::cout << "calculating automata" << std::endl;
  std::random_device rd;
  std::mt19937_64 gen(rd());

  QVector3D pos;

  std::vector<unsigned int> newAutoParticles;
  newAutoParticles.push_back(m_ID);

  std::uniform_real_distribution<float> distributionX (-6,6);
  std::uniform_real_distribution<float> distributionY (-6,6);
  std::uniform_real_distribution<float> distributionZ (-6,6);

  float x= distributionX(gen);
  float y= distributionY(gen);
  float z= distributionZ(gen);

  pos[0] = x;
  pos[1] = y;
  pos[2] = z;

  _particleList.push_back(std::unique_ptr<AutomataParticle> (new AutomataParticle(x,y,z,newAutoParticles)));
  }

  particleRules(_particleList);

  m_tick++;

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

      if(length <= m_size*10)
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

  if(neighbourCount<2 || neighbourCount>3)
  {
    m_alive = false;
  }
}

//void AutomataParticle::killParticles(std::vector<std::unique_ptr<Particle> > &_particleList)
//{
//  particleRules(_particleList);

//  for(unsigned int i=0; i<_particleList.size(); i++)
//  {
//    if(m_alive == false)
//    {
//      _particleList[i]->deleteParticle(m_ID);
//    }
//  }
//}
