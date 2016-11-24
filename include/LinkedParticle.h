#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

#include<QVector3D>

class LinkedParticle
{
public:
  LinkedParticle();// default constructor placing the particle at the 0,0,0
  LinkedParticle(qreal _x,qreal _y,qreal _z);
  void setPos(qreal _x,qreal _y,qreal _z);
  void getPos(QVector3D* _vector);
private:
  QVector3D m_pos;
};

#endif // LINKEDPARTICLE_H
