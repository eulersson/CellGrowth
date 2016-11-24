#include "include/ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
  m_particles.resize(1000);
  m_particleCount=0;
}

ParticleSystem::ParticleSystem(int size)
{
  m_particles.resize(size);
  m_particleCount=0;
}

void ParticleSystem::fill(int partAmount)
{
  for(int i=0;i<partAmount;i++)
  {
    m_particles.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle));
  }
}
