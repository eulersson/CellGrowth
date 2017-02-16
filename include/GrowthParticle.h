#ifndef GROWTHPARTICLE_H
#define GROWTHPARTICLE_H

#include"Particle.h"

////////////////////////////////////////////////////////////////////////////////
/// @file GrowthParticle.h
/// @author Carola Gille
/// @version 0.0.1
/// @class GrowthParticle
/// @brief Growth Particle inheriting from  Particle class, imitates plant like growth
////////////////////////////////////////////////////////////////////////////////
///
class GrowthParticle: public Particle
{
public:
  GrowthParticle();
  GrowthParticle(
      qreal _x,
      qreal _y,
      qreal _z);

  GrowthParticle(
      qreal _x,
      qreal _y,
      qreal _z,
      std::vector<unsigned int> _connectedParticles);

      void split(QVector3D _lightDirection,std::vector<std::unique_ptr<Particle> > &_particleList) override;



private:
  int m_childrenTreshold;
  bool collision(int _levels,QVector3D _testPosition,std::vector<std::unique_ptr<Particle>> &_particleList);

  bool testCollision(QVector3D _particle,float _size);

  bool recursiveCollision(QVector3D _particle,std::vector<std::unique_ptr<Particle>> &_particleList) override;

};



#endif // GROWTHPARTICLE_H
