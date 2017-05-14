
////////////////////////////////////////////////////////////////////////////////
/// @file GrowthParticle.cpp
/// @author Carola Gille
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Standard
#include <cmath>
#include <iostream>
#include <random>

// Project
#include "GrowthParticle.h"

GrowthParticle::GrowthParticle() : Particle()
{
  qDebug("Growth Particle default constructor.");
}

GrowthParticle::GrowthParticle(
    qreal _x,
    qreal _y,
    qreal _z,
    float _size)
  : Particle(_x, _y, _z, _size)
{
  m_childrenTreshold = 3;
  m_branchLength = 1.0;
  qDebug("Growth Particle constructor passing in position: %f,%f,%f.", _x, _y, _z);
}

GrowthParticle::GrowthParticle(
    qreal _x,
    qreal _y,
    qreal _z,
    std::vector<uint> _connectedParticles,
    float _size,
    float _branchLength)
  : Particle(_x, _y, _z, _connectedParticles, _size)
{
  m_childrenTreshold = 3;
  m_branchLength = _branchLength;
  qDebug("Growth Particle constructor passing in positions: %f,%f,%f and a list"
         " of particles.", _x, _y, _z);
}

bool GrowthParticle::split(
    QVector3D _lightPos,
    std::vector<std::unique_ptr<Particle>> &_particleList,
    std::mt19937_64 _gen,
    bool _growToLight)
{
  // Checks length of children particle list to see if the max particle threshold
  // is reached or not.
  if (m_connectedParticles.size() >= m_childrenTreshold) return false;

  // Creating a list of particles for new particles, those will represent the
  // branches between the particles
  std::vector<uint> newConnectedParticles;
  // Appends mother ID, which is always the first element in the
  // connectedParticle vector
  newConnectedParticles.push_back(m_ID);

  float input0A;
  float input0B;
  float input1A;
  float input1B;
  float input2A;
  float input2B;

  // If it isn't meant to grow towards the light it will chose a random value
  // between the parents position and twice that value.
  if (_growToLight==false)
  {
    input0A = m_pos[0] + 0.001;
    input0B = m_pos[0] + m_pos[0];
    input1A = m_pos[1] + 0.001;
    input1B = m_pos[1] + m_pos[1];
    input2A = m_pos[2] + 0.001;
    input2B = m_pos[2] + m_pos[2];
  }
  // If the particle is meant to grow towards the light it will find a value
  // between the mothers position and the light.
  else
  {
    input0A = m_pos[0] + 0.001;
    input0B = _lightPos[0];
    input1A = m_pos[1] + 0.001;
    input1B = _lightPos[1];
    input2A = m_pos[2] + 0.001;
    input2B = _lightPos[2];
  }

  // Finding a random position in the predefined boundaries.
  std::uniform_real_distribution<float> distributionX(input0A, input0B);
  std::uniform_real_distribution<float> distributionY(input1A, input1B);
  std::uniform_real_distribution<float> distributionZ(input2A, input2B);

  QVector3D pos;

  uint counter = 0;
  float branchMultiplier = 1.05;

  do {
    pos[0] = distributionX(_gen);
    pos[1] = distributionY(_gen);
    pos[2] = distributionZ(_gen);

    QVector3D direction;
    direction[0]=pos[0]-m_pos[0];
    direction[1]=pos[1]-m_pos[1];
    direction[2]=pos[2]-m_pos[2];

    // Place new particle in direction of vector mutilplied by size of particle.
    direction.normalize();

    pos[0] = m_pos[0] + direction[0] * (m_size + m_branchLength + branchMultiplier);
    pos[1] = m_pos[1] + direction[1] * (m_size + m_branchLength + branchMultiplier);
    pos[2] = m_pos[2] + direction[2] * (m_size + m_branchLength + branchMultiplier);

    // Increases the length of a branch the particle is still colliding after 50 tries.
    if(counter % 50 == 0)
    {
      branchMultiplier += 0.5;
    }
    counter++;
  }
  while(collision(4, pos, _particleList));

  // Create new particle and add to particle list
  _particleList.push_back(
    std::unique_ptr<GrowthParticle> (
      new GrowthParticle(pos[0], pos[1], pos[2], newConnectedParticles, m_size, m_branchLength)
    )
  );

  // Add particle to links in mother particle
  uint new_ID = _particleList[_particleList.size() - 1]->getID();
  m_connectedParticles.push_back(new_ID);
  return true;
}

bool GrowthParticle::collision(int _levels,QVector3D _testPosition,std::vector<std::unique_ptr<Particle> > &_particleList)
{
  // Original parent is current particle
  uint parent = m_ID;

  std::vector<uint> links;

  // Finding parent of particles until level of generation is reached.
  for (int j = 0; j <= _levels; j++)
  {
    // Finds parent
    _particleList[parent]->getConnectionsID(links);

    // Incase it does not have a parent
    if (links.size() == 0) break;

    // Incase particle is first particle ever created
    if (parent == 0) break;

    parent = links[0];
  }

  bool collision = _particleList[parent]->recursiveCollision(_testPosition, _particleList);
  return collision;

}

bool GrowthParticle::testCollision(QVector3D _particlePos)
{
  float distance = _particlePos.distanceToPoint(m_pos);
  return distance <= m_size * 2.0;
}

bool GrowthParticle::recursiveCollision(
    QVector3D _particle,
    std::vector<std::unique_ptr<Particle>> &_particleList)
{
  // Tests for collision of the current particle
  if (testCollision(_particle))
  {
     return true;
  }

  // If particle first particle ever created (doesn't have a parent)
  int start_i = 0;

  // Starting from 1 as first particle is mother particle
  if (m_ID != 0)
  {
    // If it doesn't have any children
    if(m_connectedParticles.size() <= 1) return false;
    start_i = 1;
  }

  for(size_t i = start_i; i < m_connectedParticles.size(); i++)
  {
    // If colliding return true
    if(_particleList[m_connectedParticles[i]]->recursiveCollision(_particle,_particleList)) return true;
  }
  return false;
}

void GrowthParticle::setChildThreshold(uint _amount)
{
  m_childrenTreshold=_amount;
}

void GrowthParticle::setBranchLength(float _value)
{
  m_branchLength=_value;
}
