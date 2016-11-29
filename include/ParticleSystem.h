#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

// Native
#include <vector>
#include <memory>

// Custom
#include "LinkedParticle.h"

class ParticleSystem
{
public:
  ParticleSystem();
  ParticleSystem(int _rings, int _sectors, float _radius);
  void advance();
  void fill(int _rings, int _sectors, float _radius);
  LinkedParticle* get_particle(unsigned int _idx);
  unsigned int get_size();

private:
  std::vector<std::unique_ptr<LinkedParticle>> m_particles;
  unsigned int m_particleCount;
};

#endif // PARTICLESYSTEM_H
