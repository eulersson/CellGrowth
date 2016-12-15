#include <math.h>
#include <iostream>
#include "include/ParticleSystem.h"

// Default constructor creates a 2500 (50*50) distribution of particles
ParticleSystem::ParticleSystem()
{
  std::cout<<"Calling Default Constructor\n";
  m_particleCount=0;
  fill(4);
}

// For custom number of particles
ParticleSystem::ParticleSystem(int _amount)
{
  std::cout<<"Calling Costum Constructor\n";
  m_particleCount=0;
  fill(_amount);

}

// Calculates new forces on each particles and advects them
void ParticleSystem::advance()
{
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate();
  }

  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->advance();
  }
}

// Starting with 4 particles that are all linked together
void ParticleSystem::fill(int _amount)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_real_distribution<float> distribution(0,0.5);


  for(int i=0;i<_amount;i++)
  {

    qreal x=distribution(gen);
    qreal y=distribution(gen);
    qreal z=distribution(gen);

    m_particles.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle(x, y, z)));
    m_particleCount++;
  }

  if(_amount<=4)
  {
    // we start with a small amount of particles so they can all be linked to each other
    for(int i=0;i<m_particles.size();i++)
    {
      //linking all other particles to the i particle
      for(int j=0;j<m_particles.size();j++)
      {
        if(j==i){continue;}
        m_particles[i]->link(m_particles[j]->getID());

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
LinkedParticle* ParticleSystem::get_particle(unsigned int _idx)
{
  return m_particles[_idx].get();
}

// Gets the total number of particles
unsigned int ParticleSystem::get_size()
{
  //changed this to querz the size
  return m_particles.size();
}

void ParticleSystem::getLinksForDraw(std::vector<QVector3D> &_returnList)
{
  _returnList.clear();
  //there is a lot of iterating here maybe there can be found a better way to do this (wow my english..)
  // using pointers would maybe get rid of the the iterations but it might cause problems with vectors reallocating
  //their memory as the memory address changes we could have a fixed size array and maybe have a max particle treshold
  //Using size of vetor for this because jon said i should be doing that we can change it to particle Count though
  for(int i=0;i<m_particles.size();i++)
  {
    //gets the links from the current particle and than looks for the position dependent on the particles ID
    std::vector<int> tempList;
    m_particles[i]->getLinks(tempList);
    for(int j=0;j<tempList.size();j++)
    {

      //adding the positions to the return list making every second item the position of the current partcile so a line can be drawn
      for(int k=0;k<m_particles.size();k++)
      {
        if(m_particles[k]->getID()==tempList[j])
        {

          QVector3D vec;
          //pushes back the pos of Linked Particle
          m_particles[k]->getPos(vec);
          _returnList.push_back(vec);
          //pushes back the pos of current Particle
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
  //WIP
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_real_distribution<float> distribution(0,m_particles.size());

  m_particles[distribution(gen)]->split(m_particles);

  QVector3D vec;
  m_particles[m_particles.size()-1]->getPos(vec);
  std::cout<<"actual position "<<vec.x()<<','<<vec.y()<<','<<vec.z()<<std::endl;
  m_particleCount++;

}
