#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

#include<QVector3D>

class LinkedParticle
{
public:
  LinkedParticle();
  LinkedParticle(qreal _x,qreal _y,qreal _z);

  // Updates the position after forces (velocities) are computed
  void advance();

  // Computes the forces that act on this particle
  void calculate();

  // Writes the position to the QVector3D that is passed by reference
  void getPos(QVector3D &_vector);

  // Sets the current position of the linked particle
  void setPos(qreal _x,qreal _y,qreal _z);

private:
  QVector3D m_pos;
  QVector3D m_vel;
};

#endif // LINKEDPARTICLE_H
