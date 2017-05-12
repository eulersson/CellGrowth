////////////////////////////////////////////////////////////////////////////////
/// @file LinkedParticle.h
/// @author Carola Gille
/// @author Ramon Blanquer
/// @author Esme Prior
/// @author Lydia Kenton
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
  /// @brief Calculates the new velocity of the particle based on the forces
  /// that act on it.
  /// @param [in] _particleList List of all particles
  /// @param [in] _averageDistance Average distance between particles
  /// @param [in] _particleCount Total number of particles in the system
  /// @param [in] _lightPos Holds the position of the point light
  /// @param [in] _cohesionFactor Controls the strength of cohesion
  /// @param [in] _localCohesionFactor Controls the strength of local cohesion
  /// @param [in] _particleDeath Toggles whether or not particle death is true
  /// @param [in] _automataRadius Controls the radius in which automata are created
  /// @param [in] _automataTime Controls the speed at which automata are created
  //////////////////////////////////////////////////////////////////////////////
  void calculate(std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance,
                 unsigned int _particleCount,QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor,
                 bool _particleDeath, int _automataRadius, int _automataTime) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Repulses the particles which aren't connected by links to
  /// avoid collisions.
  /// @param [in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  void calculateUnlinked(std::vector<std::unique_ptr<Particle>> &_particleList);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Moves the particles closest to the centre to create a bulge effect.
  /// @param [in] _particleCentre Position of the average centre of all particles
  //////////////////////////////////////////////////////////////////////////////
  void bulge(QVector3D _particleCentre) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Adds food to random particles to create interesting effects.
  /// @param [in] _particleCentre Position of the average centre of all particles
  //////////////////////////////////////////////////////////////////////////////
  void addFood(QVector3D _particleCentre) override;

  // Computes all the relinking and creates a new particle
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Called when particle needs to be split, Calculates which particles
  /// are linked to the new and which to the old particle.
  /// @param[in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  bool split(std::vector<std::unique_ptr<Particle> > &_particleList, std::mt19937_64 _gen) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Double checks that all links go both ways, and if not, creates new
  /// connections.
  /// @param[in] _ID Holds the unique ID of the particle.
  /// @param[in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////

  void doubleConnect(unsigned int _ID, std::vector<std::unique_ptr<Particle> > &_particleList) override;

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
  int m_particleLife;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Value for the lifespan controlled by food.
  //////////////////////////////////////////////////////////////////////////////
  int m_foodLife;

};

#endif // LINKEDPARTICLE_H
