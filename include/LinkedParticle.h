#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

// Project
#include "Particle.h"

////////////////////////////////////////////////////////////////////////////////
/// @file LinkedParticle.h
/// @author Carola Gille
/// @author Ramon Blanquer
/// @version 0.0.1
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
  //////////////////////////////////////////////////////////////////////////////
  LinkedParticle(qreal _x,
                 qreal _y,
                 qreal _z);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor allowing user input for position as well as
  /// which particles it is connected to.
  /// @param[in] _x x Position of the particle.
  /// @param[in] _y y Position of the particle.
  /// @param[in] _z z Position of the particle.
  /// @param[in] _linkedParticles List of particle IDs to be connected to
  /// the newly generated particle.
  //////////////////////////////////////////////////////////////////////////////
  LinkedParticle(qreal _x,
                 qreal _y,
                 qreal _z,
                 std::vector<unsigned int> _linkedParticles);

  // Lydia and Esmes function
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Needs description.
  //////////////////////////////////////////////////////////////////////////////
  void calculate() override;

  // Computes all the relinking and creates a new particle
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Called when particle needs to be split, Calculates which particles
  /// are linked to the new and which to the old particle.
  /// @param[in] _particleList List of all particles
  //////////////////////////////////////////////////////////////////////////////
  void split(std::vector<std::unique_ptr<Particle> > &_particleList) override;

private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Tests for position relative to a plane.
  /// @param[in] _normal Normal of the plane
  /// @param[in] _planePoint Position on the plane to define the plane
  /// @param[in] _testPoint Position to be tested
  /// @brief return Returns the distance to the plane
  //////////////////////////////////////////////////////////////////////////////
  int planeSorting(QVector3D _normal,QVector3D _planePoint,QVector3D _testPoint);

};

#endif // LINKEDPARTICLE_H
