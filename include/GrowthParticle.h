#ifndef GROWTHPARTICLE_H
#define GROWTHPARTICLE_H

#include"Particle.h"

////////////////////////////////////////////////////////////////////////////////
/// @file GrowthParticle.h
/// @author Carola Gille
/// @version 0.0.1
/// @class GrowthParticle
/// @brief Growth Particle Inheriting from  Particle class, imitates plant like
/// growth.
////////////////////////////////////////////////////////////////////////////////
class GrowthParticle: public Particle
{

public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Default constructor placing particle at the origin.
  //////////////////////////////////////////////////////////////////////////////
  GrowthParticle();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor allowing user input for position.
  /// @param[in] _x x Position of the particle.
  /// @param[in] _y y Position of the particle.
  /// @param[in] _z z Position of the particle.
  //////////////////////////////////////////////////////////////////////////////
  GrowthParticle(
      qreal _x,
      qreal _y,
      qreal _z);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor allowing user input for position as well as
  /// which particles it is connected to.
  /// @param[in] _x x Position of the particle.
  /// @param[in] _y y Position of the particle.
  /// @param[in] _z z Position of the particle.
  /// @param[in] _connectedParticles List of particle IDs to be connected to
  /// the newly generated particle.
  //////////////////////////////////////////////////////////////////////////////
  GrowthParticle(
      qreal _x,
      qreal _y,
      qreal _z,
      std::vector<unsigned int> _connectedParticles);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Does not do anything for this class.
  //////////////////////////////////////////////////////////////////////////////
  void calculate() override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Called when particle needs to be split and creates a new branch from that Particle.
  /// @param[in] _lightDirection Light direction.
  /// @param[in] _particleList List of all particles.
  //////////////////////////////////////////////////////////////////////////////
  void split(
      QVector3D _lightDirection,
      std::vector<std::unique_ptr<Particle>> &_particleList) override;


  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the child threshold
  /// @param[in] _amount amount of children allowed per branch.
  //////////////////////////////////////////////////////////////////////////////
  void setChildThreshold(int _amount);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the branch length of a branch.
  /// @param[in] _value length of the branch.
  //////////////////////////////////////////////////////////////////////////////
  void setBranchLength(float _value);








private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief hold the threshold of how many children/branches one Particle can have.
  //////////////////////////////////////////////////////////////////////////////
  unsigned int m_childrenTreshold;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Finds the parent according to the levels to call the recursive
  /// collsion on it.
  /// @param[in] _levels represents the levels of collision testing it will do.
  /// 1 level is the equivalent of one generation earlier.
  /// @param[in] _testPosition the position that needs collision testing.
  /// @param[in] _particleList List of all particles.
  /// @returns returns true if colliding and false if it's not colliding.
  //////////////////////////////////////////////////////////////////////////////
  bool collision(
      int _levels,
      QVector3D _testPosition,
      std::vector<std::unique_ptr<Particle>> &_particleList);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Tests directly for one on one collision between the particle and the
  /// input particle position.
  /// @param[in] _particlePos position of the particle that is meant to be tested
  /// against.
  //////////////////////////////////////////////////////////////////////////////
  bool testCollision(QVector3D _particlePos);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Recursively calling on parent to run collisions on all children of
  ///  a particle.
  /// @param[in] _particle Parent Particle.
  /// @param[in] _particleList List of all Particles.
  //////////////////////////////////////////////////////////////////////////////
  bool recursiveCollision(
      QVector3D _particle,
      std::vector<std::unique_ptr<Particle>> &_particleList) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Length of a branches connecting to the particle
  //////////////////////////////////////////////////////////////////////////////
  float m_branchLength;

};

#endif // GROWTHPARTICLE_H
