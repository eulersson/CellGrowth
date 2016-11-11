#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#ifdef __CUDACC__
#define CUDA_HOST __host__
#define CUDA_DEVICE __device__
#define CUDA_HOST_AND_DEVICE __host__ __device__
#else
#define CUDA_HOST
#define CUDA_DEVICE
#define CUDA_HOST_AND_DEVICE
#endif

#include <vector>
#include <thrust/device_vector.h>

class ParticleSystem {
public:
    ParticleSystem();
    void birth_particle();
    void advance_particles(unsigned int VBO_id);
    thrust::device_vector<float> positions;
    thrust::device_vector<float> velocities;
};

#endif // PARTICLESYSTEM_H
