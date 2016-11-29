#include <math.h>

#include "include/ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
    fill(50, 50, 0.3);
}

ParticleSystem::ParticleSystem(int _rings, int _sectors, float _radius)
{
  fill(_rings, _sectors, _radius);
}

void ParticleSystem::advance()
{
  for (unsigned int i = 0; i < m_particleCount; ++i)
  {
    m_particles[i]->calculate();
    m_particles[i]->advance();
  }
}

void ParticleSystem::fill(int _rings, int _sectors, float _radius)
{
  m_particleCount = _rings * _sectors;

  float const R = 1.0f / (float)(_rings - 1);
  float const S = 1.0f / (float)(_sectors - 1);

  for (unsigned int r = 0; r < _rings; ++r)
  {
    for (unsigned int s = 0; s < _sectors; ++s)
    {
      float y = sin( -M_PI_2 + M_PI * r * R );
      float x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
      float z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
      x *= _radius; y *= _radius; z *= _radius;
      m_particles.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle(x, y, z)));
    }
  }
}

LinkedParticle* ParticleSystem::get_particle(unsigned int _idx)
{
  return m_particles[_idx].get();
}

unsigned int ParticleSystem::get_size()
{
  return m_particleCount;
}
