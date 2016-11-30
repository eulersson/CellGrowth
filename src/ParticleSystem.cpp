#include <math.h>

#include "include/ParticleSystem.h"

// Default constructor creates a 2500 (50*50) distribution of particles
ParticleSystem::ParticleSystem()
{
    fill(4, 4, 0.1);
}

// For custom number of particles
ParticleSystem::ParticleSystem(int _rings, int _sectors, float _radius)
{
  fill(_rings, _sectors, _radius);
}

// Calculates new forces on each particles and advects them
void ParticleSystem::advance()
{
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate();
    m_particles[i]->advance();
  }
}

// Initializes the system with a spherical distribution of particles. This will be
// replaced with a geodesic dome sphere (different topology)
void ParticleSystem::fill(int _rings, int _sectors, float _radius)
{
  m_particleCount = _rings * _sectors;

  float const R = 1.0f / (float)(_rings - 1);
  float const S = 1.0f / (float)(_sectors - 1);

  for (unsigned int r = 0; r < _rings; ++r)
  {
    for (unsigned int s = 0; s < _sectors; ++s)
    {
      //float y = sin( -M_PI_2 + M_PI * r * R );
      //float x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
      //float z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
      float y = 2;
      float x = 2;
      float z = 2;
      x *= _radius; y *= _radius; z *= _radius;
      m_particles.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle(x, y, z)));
    }
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
