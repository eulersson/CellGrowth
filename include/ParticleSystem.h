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

  // Steps all the particles in the system (calls internally the advance() method of each particle)
  void advance();

  // Initializes _rings * _sectors number of particles
  void fill(int _rings, int _sectors, float _radius);

  // Retrieves the particle at that index, returns a pointer
  LinkedParticle* get_particle(unsigned int _idx);

  // Get the number of particles in the system
  unsigned int get_size();

private:
  std::vector<std::unique_ptr<LinkedParticle>> m_particles;
  unsigned int m_particleCount;
};

#endif // PARTICLESYSTEM_H
