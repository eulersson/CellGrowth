#include <iostream>

#include "particlesystem.h"

void modifying_vbo(unsigned int VBO_id, ParticleSystem* ps);

ParticleSystem::ParticleSystem()
{


}

void ParticleSystem::birth_particle()
{
    positions.push_back(0.0f);
    positions.push_back(0.5f);
    positions.push_back(0.0f);
    velocities.push_back(0.01f *  (2*((float)rand()/RAND_MAX)-1));
    velocities.push_back(0.01f *  (2*((float)rand()/RAND_MAX)-1));
    velocities.push_back(0.01f *  (2*((float)rand()/RAND_MAX)-1));
}

void ParticleSystem::advance_particles(unsigned int VBO_id)
{
    modifying_vbo(VBO_id, this);

}
