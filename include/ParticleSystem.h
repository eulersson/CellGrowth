#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <memory>

#include "LinkedParticle.h"

class ParticleSystem
{
public:
  ParticleSystem();//default size 1000
  ParticleSystem(int size);
  void fill(int partAmount);

private:
  std::vector<std::unique_ptr<LinkedParticle>> m_particles;
  int m_particleCount;
};

#endif // PARTICLESYSTEM_H
