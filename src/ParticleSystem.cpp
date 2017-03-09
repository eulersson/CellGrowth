// Native
#include <math.h>
#include <iostream>
#include <random>

// Custom
#include "include/ParticleSystem.h"

// Default constructor creates a 2500 (50*50) distribution of particles
ParticleSystem::ParticleSystem()
{
  qDebug("Default constructor called");
  m_particleCount=0;
  fill(3);
  m_particleCentre = calculateParticleCentre();
}

// For custom number of particlesm_packagedParticleData
ParticleSystem::ParticleSystem(int _amount)
{
  qDebug("Custom constructor called");
  m_particleCount=0;
  fill(_amount);
  m_particleCentre = calculateParticleCentre();

}

// Calculates new forces on each particles and advects them
void ParticleSystem::advance()
{
  m_particleCount=m_particles.size();
  // First splitting
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
//    if(m_particles[i]->testForSplit())
//    {
//      m_particles[i]->split(m_particles);
//    }
  }

  // Then moving
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate(m_particleCentre, m_particles);
  }

  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->advance();
  }
}

// Starting with 4 particles that are all linked together
void ParticleSystem::fill(unsigned int _amount)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_real_distribution<float> distribution(-10.0,10.0);
  std::vector<QVector3D> pos;
  pos.push_back(QVector3D(0.25,0.25,0));
  pos.push_back(QVector3D(0.25,-0.25,0));
  pos.push_back(QVector3D(-0.25,-0.25,0));

  for (unsigned int i = 0; i < _amount; i++)
  {
    //qreal x=distribution(gen);
    //qreal y=distribution(gen);
    //qreal z=distribution(gen);
    //qreal z = -25.0f;

    //m_particles.emplace_back(std::unique_ptr<Particle>(new LinkedParticle(x, y, z)));
    m_particles.emplace_back(std::unique_ptr<Particle>(new LinkedParticle(pos[i].x(),pos[i].y(),pos[i].z())));
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
    //run a triangulation algorithm
    //This will need to be added later
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

void ParticleSystem::getLinksForDraw(std::vector<QVector3D> &_returnList)
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
          QVector3D vec;

          // Pushes back the pos of Linked Particle
          m_particles[k]->getPos(vec);
          _returnList.push_back(vec);

          // Pushes back the pos of current Particle
          m_particles[i]->getPos(vec);
          _returnList.push_back(vec);
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

  QVector3D light(-100*sin(m_particleCount*10),- 100,100+sin(m_particleCount*10));

  // !!!!!!  ATTENTION SPLIT FUNCTION SHOULD BE BASED ON PARTICLE TYPE

  m_particles[distribution(gen)]->split(m_particles);
  m_particleCount++;
//  QVector3D vec;
//  m_particles[m_particles.size() - 1]->getPos(vec);

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
    //std::cout<<"ParticlePosition: "<<particlePosition.x()<<" "<<particlePosition.y()<<" "<<particlePosition.z()<<std::endl;
    m_particleCentre += particlePosition;

  }

  m_particleCentre = m_particleCentre/(m_particles.size());
  return m_particleCentre;
}

std::vector<QVector3D> ParticleSystem::listOfPositions()
{
  std::vector<QVector3D> _listOfPositions;

  for (auto &particle : m_particles)
  {
    _listOfPositions.push_back(particle->getPosition());
  }

  return _listOfPositions;
}


