////////////////////////////////////////////////////////////////////////////////
/// @file LinkedParticle.h
/// @author Carola Gille
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

// Project
#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////
/// @class LinkedParticle
/// @brief Inherits from Particle class, Linked particles Cell growth based on
/// surfaces.
////////////////////////////////////////////////////////////////////////////////
class LinkedParticle : public Particle
{

public:

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Default constructor placing particle at the origin.
  //////////////////////////////////////////////////////////////////////////////
  LinkedParticle();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor allowing user input for position.
  /// @param[in] _x x Position of the particle.
  /// @param[in] _y y Position of the particle.
  /// @param[in] _z z Position of the particle.
  /// @param[in] _size size of particle
  //////////////////////////////////////////////////////////////////////////////
  LinkedParticle(qreal _x,
                 qreal _y,
                 qreal _z,
                 float _size);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor allowing user input for position as well as
  /// which particles it is connected to.
  /// @param[in] _x x Position of the particle.
  /// @param[in] _y y Position of the particle.
  /// @param[in] _z z Position of the particle.
  /// @param[in] _linkedParticles List of particle IDs to be connected to
  /// the newly generated particle.
  /// @param[in] _size size of particle
  //////////////////////////////////////////////////////////////////////////////
  LinkedParticle(qreal _x,
                 qreal _y,
                 qreal _z,
                 std::vector<unsigned int> _linkedParticles,
                 float _size);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Calculates the particle new position based on Forces.
  /// @param [in] _particleCentre Position of the average centre of all particles
  /// @param [in] _particleList List of all particles
  /// @param [in] _averageDistance Average distance between particles
  /// @param [in] _particleCount Total number of particles in the system
  /// @param [in] _lightPos Holds the position of the point light
  /// @param [in] _cohesionFactor Holds the amount of cohesion taken from the slider.
  /// @param [in] _localCohesionFactor Holds the amount of local cohesion taken from the slider.
  //////////////////////////////////////////////////////////////////////////////
  void calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance, unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor, bool _particleDeath) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Repulses the particles which aren't connected by links to
  /// avoid collisions.
  /// @param [in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  void calculateUnlinked(std::vector<std::unique_ptr<Particle>> &_particleList);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Calculates the spring and hold functions between linked particles.
  /// @param [in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  //void spring(std::vector<std::unique_ptr<Particle>> &_particleList, int _springFactor);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Moves the particles closest to the centre to create a bulge effect.
  /// @param [in] _particleCentre Position of the average centre of all particles
  //////////////////////////////////////////////////////////////////////////////
  void bulge(QVector3D _particleCentre) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Moves the particles being hit by light towards the point light
  /// @param [in] _particleList List of all particles
  /// @param [in] _lightPos Holds the position of the point light
  //////////////////////////////////////////////////////////////////////////////
  void lightAttract(std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _lightPos);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns a list containing all particles closest to the light.
  /// @param [in] _particleList List of all particles
  /// @param [in] _lightPos Holds the position of the point light
  /////////////////////////////////////////////////////////////////////////////
  std::vector<unsigned int> getHitParticles(std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _lightPos);

  // Computes all the relinking and creates a new particle
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Called when particle needs to be split, Calculates which particles
  /// are linked to the new and which to the old particle.
  /// @param[in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  void split(std::vector<std::unique_ptr<Particle> > &_particleList, std::mt19937_64 _gen) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Double checks that all links go both ways, and if not, creates new
  /// connections.
  /// @param[in] _ID Holds the unique ID of the particle.
  /// @param[in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  void connect(unsigned int _ID, std::vector<std::unique_ptr<Particle> > &_particleList) override;

private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Tests for position relative to a plane.
  /// @param[in] _normal Normal of the plane
  /// @param[in] _planePoint Position on the plane to define the plane
  /// @param[in] _testPoint Position to be tested
  /// @brief return Returns the distance to the plane
  //////////////////////////////////////////////////////////////////////////////
  int planeSorting(QVector3D _normal,QVector3D _planePoint,QVector3D _testPoint);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Increases with time, to check how long the particle has been alive.
  //////////////////////////////////////////////////////////////////////////////
  int particleLife;

};

#endif // LINKEDPARTICLE_H
