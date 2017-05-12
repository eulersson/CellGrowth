////////////////////////////////////////////////////////////////////////////////
/// @file AutomataParticle.h
/// @author Lydia Kenton
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#ifndef AUTOMATAPARTICLE_H
#define AUTOMATAPARTICLE_H

// Project
#include "Particle.h"

//Qt
#include <QTime>

////////////////////////////////////////////////////////////////////////////////
/// @class AutomataParticle
/// @brief Automata Particle inheriting from Particle class, imitates
/// Conway's Game of Life.
////////////////////////////////////////////////////////////////////////////////
class AutomataParticle : public Particle
{
public :

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Default constructor placing particle at the origin.
  //////////////////////////////////////////////////////////////////////////////
  AutomataParticle();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor allowing user input for position.
  /// @param[in] _x x Position of the particle.
  /// @param[in] _y y Position of the particle.
  /// @param[in] _z z Position of the particle.
  //////////////////////////////////////////////////////////////////////////////
  AutomataParticle(qreal _x,
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
  AutomataParticle(qreal _x,
                 qreal _y,
                 qreal _z,
                 std::vector<unsigned int> _automataParticles);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Calculates the new velocity of the particle based on the forces
  /// that act on it.
  /// @param _particleCentre Position of the average centre of all particles
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
  void calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance,
                 unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor,
                 bool _particleDeath, int _automataRadius, int _automataTime);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Finds the neighbours of the particles.
  /// @param [in] _particleList List of all particles.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<unsigned int> getNeighbours(std::vector<std::unique_ptr<Particle> > &_particleList);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Defines the rules based on Conway's Game of Life.
  /// @param [in] _particleList List of all particles.
  //////////////////////////////////////////////////////////////////////////////
  void particleRules(std::vector<std::unique_ptr<Particle> > &_particleList);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Boolean to define when a particle is alive.
  /// @returns Flag for whether or not particle is alive.
  //////////////////////////////////////////////////////////////////////////////
  virtual bool isAlive(){return m_alive;}

private:

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Stores the time using QTime.
  //////////////////////////////////////////////////////////////////////////////
  QTime m_time;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Stores the radius of automata particle birth.
  //////////////////////////////////////////////////////////////////////////////
  int m_rad;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Boolean to flag if the particle is alive.
  //////////////////////////////////////////////////////////////////////////////
  bool m_alive;

};

#endif AUTOMATAPARTICLE_H
