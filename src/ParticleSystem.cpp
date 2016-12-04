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

  for(int i=0;i<_amount;i++)
  {
    qreal x=float(std::rand() % 10)/30;
    qreal y=float(std::rand() % 10)/30;
    qreal z=float(std::rand() % 10)/30;
    m_particles.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle(x, y, z)));
    m_particleCount++;
    std::cout<<m_particles[i]->getID()<<"\n";
    std::cout<<"i= "<<i<<"\n";
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
  return m_particleCount;
}
