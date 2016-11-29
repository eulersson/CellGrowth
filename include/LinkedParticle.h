#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

#include<QVector3D>

class LinkedParticle
{
public:
  LinkedParticle();
  LinkedParticle(qreal _x,qreal _y,qreal _z);
  void advance();
  void calculate();
  void getPos(QVector3D &_vector);
  void setPos(qreal _x,qreal _y,qreal _z);

private:
  QVector3D m_pos;
  QVector3D m_vel;
};

#endif // LINKEDPARTICLE_H
