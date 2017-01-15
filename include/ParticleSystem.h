#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

// Native
#include <vector>

// Custom
#include "LinkedParticle.h"

class ParticleSystem
{
public:
  // Constructors
  ParticleSystem();
  ParticleSystem(int _amount);

  // Steps all the particles in the system (calls internally the advance() method of each particle)
  void advance();

  // Initializes _rings * _sectors number of particles
  void fill(unsigned int _amount);

  // Retrieves the particle at that index, returns a pointer
  LinkedParticle* get_particle(unsigned int _idx);

  // Get the number of particles in the system
  unsigned int get_size();

  // Returns positions ready to draw type QVector3D might need to be changed
  void getLinksForDraw(std::vector<QVector3D> &_returnList);
  void splitRandomParticle();

  // This will delete a partcle and all the links to that particle
  void deleteParticle(unsigned int _index);

private:
  // Stores the list of particles in the system
  std::vector<std::unique_ptr<LinkedParticle>> m_particles;

  // Keeps track of the number of particles
  unsigned int m_particleCount;
};

#endif // PARTICLESYSTEM_H
