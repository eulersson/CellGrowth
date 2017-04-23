#ifndef AUTOMATAPARTICLE_H
#define AUTOMATAPARTICLE_H

// Project
#include "Particle.h"

class AutomataParticle : public Particle
{
public :

  AutomataParticle();

  AutomataParticle(qreal _x,
                 qreal _y,
                 qreal _z);

  AutomataParticle(qreal _x,
                 qreal _y,
                 qreal _z,
                 std::vector<unsigned int> _automataParticles);

  void calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance, unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor, bool _particleDeath);

  std::vector<unsigned int> getNeighbours(std::vector<std::unique_ptr<Particle> > &_particleList);

  void particleRules(std::vector<std::unique_ptr<Particle> > &_particleList);

  void killParticles(std::vector<std::unique_ptr<Particle> > &_particleList);

  virtual bool isAlive(){return m_alive;}

private:

  int m_tick;

  bool m_alive;

};


























#endif AUTOMATAPARTICLE_H
