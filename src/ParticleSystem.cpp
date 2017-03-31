////////////////////////////////////////////////////////////////////////////////
/// @file ParticleSystem.cpp
/// @author Carola Gille
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Native
#include <math.h>
#include <iostream>
#include <random>

// Custom
#include "include/ParticleSystem.h"

// Default constructor creates a 2500 (50*50) distribution of particles
ParticleSystem::ParticleSystem()
{
  //qDebug("Default constructor called");
  m_particleCount=0;
  m_particleType= 'L';
  fill(3);
  m_forces = true;
  m_cohesion = 30; //percent
  m_spring = 30;

}

// For custom number of particlesm_packagedParticleData
ParticleSystem::ParticleSystem(char _particleType)
{
  //qDebug("Custom constructor called");
  m_particleCount=0;


  m_particleType = _particleType;

  //if it's a linked particle we need 3 particle
  if (m_particleType=='L')
  {
    fill(3);
  }
  //if it's a Growth particle we need 1 particle to start with
  else if (m_particleType== 'G')
  {
    fill(1);
    m_forces = true;
    m_cohesion = 30; //percent
    m_spring = 30;
  }

}


void ParticleSystem::advance()
{
  //reseting the particle count to the size of the particle list
  m_particleCount=m_particles.size();

  // First splitting
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    //split only if triggered by light
//    if(m_particles[i]->testForSplit())
//    {
//      m_particles[i]->split(m_particles);
//    }
  }

  //calcuting the forces
  if (m_forces==true)
  {
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate(m_particleCentre, m_particles, m_averageDistance, m_particleCount);
  }

  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->advance();
  }
  }
}

void ParticleSystem::bulge()
{
  m_particleCount=m_particles.size();
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->bulge(m_particleCentre);
    m_particles[i]->advance();
  }
  calculateParticleCentre();
}




void ParticleSystem::fill(unsigned int _amount)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_real_distribution<float> distribution(-10.0,10.0);
  std::vector<QVector3D> pos;
  pos.push_back(QVector3D(0.5,0.5,0));
  pos.push_back(QVector3D(0.5,-0.5,0));
  pos.push_back(QVector3D(-0.5,-0.5,0));

  for (unsigned int i = 0; i < _amount; i++)
  {
    qreal x=distribution(gen);
    qreal y=distribution(gen);
    qreal z=distribution(gen);

    if(m_particleType=='G')
    {
      m_particles.emplace_back(std::unique_ptr<Particle>(new GrowthParticle(x, y, z)));
    }
    else if(m_particleType=='L')
    {
      m_particles.emplace_back(std::unique_ptr<Particle>(new LinkedParticle(x, y, z)));
    }
    m_particleCount++;
  }

  if (_amount <= 4)
  {
    // We start with a small amount of particles so they can all be linked to each other
    for (unsigned int i=0; i < m_particles.size(); i++)
    {
      // Linking all other particles to the i particle
      for (unsigned int j = 0; j < m_particles.size(); j++)
      {
        if (j == i) { continue; }
        m_particles[i]->connect(m_particles[j]->getID());
      }
    }
  }

  else
  {
    qDebug("To many particles to link");
  }
}

// Returns a NORMAL pointer to the linked particle, not a smart one, otherwise
// the copy constructor triggered by the = (assignment) operator would trigger
// a change of ownership. We do not want that. Read on unique_ptr and shared_ptr.
Particle* ParticleSystem::getParticle(unsigned int _idx)
{
  return m_particles[_idx].get();
}

// Gets the total number of particles
unsigned int ParticleSystem::getSize()
{
  // Changed this to query the size
  return m_particles.size();
}

void ParticleSystem::getLinksForDraw(std::vector<uint> &_returnList)
{
  _returnList.clear();
  // There is a lot of iterating here maybe there can be find a better way to
  // do this. Using pointers would maybe get rid of the the iterations but it
  // might cause problems with vectors reallocating their memory as the memory
  // address changes we could have a fixed size array and maybe have a max
  // particle treshold. Using size of vetor for this because Jon said I should
  // be doing that we can change it to particle Count though
  for (unsigned int i = 0; i < m_particles.size(); i++)
  {
    // Gets the links from the current particle and than looks for the position
    // dependent on the particles ID
    std::vector<unsigned int> tempList;
    m_particles[i]->getConnectionsID(tempList);
    for (unsigned int j = 0; j < tempList.size(); j++)
    {
      // Adding the positions to the return list making every second item the
      // position of the current particle so a line can be drawn
      for (unsigned int k = 0; k < m_particles.size(); k++)
      {
        if (m_particles[k]->getID() == tempList[j])
        {
          // Pushes back the ID of linked Particle
          _returnList.push_back(m_particles[k]->getID());

          // Pushes back the ID of current Particle
          _returnList.push_back(m_particles[i]->getID());
          break;
        }
      }
    }
  }
}


void ParticleSystem::splitRandomParticle()
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_real_distribution<float> distribution(0,m_particles.size());


  //needs to be replaced by the actual light direction
  QVector3D light(-100*sin(m_particleCount*10),- 100,100+sin(m_particleCount*10));

  // calling diffrent split function based on the particle type

  if(m_particleType=='G')
  {
  m_particles[distribution(gen)]->split(light,m_particles);
  }
  else if(m_particleType=='L')
  {
    m_particles[distribution(gen)]->split(m_particles);
  }

  m_particleCount++;

  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate(m_particleCentre, m_particles, m_averageDistance, m_particleCount);
  }

  qDebug("Particles: %d", m_particleCount);
}





void ParticleSystem::splitHitParticle()
{
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->getHitParticles(m_particles);
  }

  m_particles[0]->split(m_particles);
  m_particleCount++;

  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate(m_particleCentre, m_particles, m_averageDistance, m_particleCount);
  }
}

void ParticleSystem::deleteParticle(unsigned int _index)
{
  std::vector<unsigned int> deleteList;
  m_particles[_index]->getConnectionsID(deleteList);
  int ID = m_particles[_index]->getID();
  for (unsigned int i = 0; i < deleteList.size(); i++)
  {
     for (unsigned int j = 0; j < m_particles.size(); j++)
     {
       if (m_particles[j]->getID() == deleteList[i])
       {
         m_particles[j]->deleteConnection(ID);
         break;
       }
     }
  }
}

void ParticleSystem::packageDataForDrawing(std::vector<float> &_packagedData)
{
  _packagedData.clear();
  for_each(m_particles.begin(), m_particles.end(), [&_packagedData](std::unique_ptr<Particle> &p)
  {
    QVector3D pos;
    p->getPos(pos);

    float radius;
    p->getRadius(radius);

    _packagedData.push_back(pos.x());
    _packagedData.push_back(pos.y());
    _packagedData.push_back(pos.z());
    _packagedData.push_back(radius);
  });
}

QVector3D ParticleSystem::calculateParticleCentre()
{
  m_particleCentre.setX(0);
  m_particleCentre.setY(0);
  m_particleCentre.setZ(0);

  for (auto &particle : m_particles)
  {
    QVector3D particlePosition = particle->getPosition();
    m_particleCentre += particlePosition;

  }

  m_particleCentre = m_particleCentre/(m_particles.size());
  return m_particleCentre;
}

QVector3D ParticleSystem::calculateAverageDistanceFromCentre()
{
  QVector3D averageDistance;

  for (auto&particle : m_particles)
  {
    QVector3D particlePosition = particle->getPosition();
    QVector3D particleCentre = calculateParticleCentre();
    QVector3D distance = particleCentre - particlePosition;
    QVector3D fabsDistance;
    fabsDistance.setX(fabs (distance.x()));
    fabsDistance.setY(fabs (distance.y()));
    fabsDistance.setZ(fabs(distance.z()));

    averageDistance += fabsDistance;
  }

  averageDistance = averageDistance/(m_particles.size());
  //std::cout<<"averagedistance:"<<averageDistance.x()<<std::endl;
  return averageDistance;
}

void ParticleSystem::setParticleSize(double _size)
{
  for(unsigned int i=0;i< m_particles.size();i++)
  {
    m_particles[i]->setRadius(_size);
  }
}

void ParticleSystem::toggleForces(bool _state)
{
  m_forces=_state;
}

void ParticleSystem::setCohesion(int _amount)
{
  m_cohesion = _amount;
}

void ParticleSystem::setSpring(int _amount)
{
  m_spring = _amount;
}

void ParticleSystem::setBranchLength(float _amount)
{
  for(unsigned int i=0;i< m_particles.size();i++)
  {
    m_particles[i]->setBranchLength(_amount);
  }
}

void ParticleSystem::setChildThreshold(int _value)
{
  for(unsigned int i=0;i< m_particles.size();i++)
  {
    m_particles[i]->setChildThreshold(_value);
  }
}

void ParticleSystem::setGrowthRadius(int _amount)
{
  //not sure where to put either
}

void ParticleSystem::reset(char _particleType)
{
  m_particles.clear();
  m_particleCount=0;
  Particle::resetIDCounter();
  m_particleType=_particleType;
  if (m_particleType=='L')
  {
    fill(3);

    m_forces = true;
    m_cohesion = 30; //percent
    m_spring = 30;
  }
  else if (m_particleType== 'G')
  {
    fill(1);
  }
}
