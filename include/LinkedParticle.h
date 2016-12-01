#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

#include<QVector3D>

class LinkedParticle
{
public:
  LinkedParticle();
  LinkedParticle(qreal _x,qreal _y,qreal _z);
  LinkedParticle(qreal _x,qreal _y,qreal _z,std::vector<int> _linkedParticles);
  // Updates the position after forces (velocities) are computed only called after all new position for particles are calculated

  void advance();

  // Computes the forces that act on this particle
  void calculate();

  // Writes the position to the QVector3D that is passed by reference
  void getPos(QVector3D &_vector);

  // Sets the current position of the linked particle
  void setPos(qreal _x,qreal _y,qreal _z);

  void link(int _ID);
  void deleteLinke(int _ID);
  int getID();
private:
  QVector3D m_pos;
  QVector3D m_vel;
  static int m_ID_counter;
  int m_ID;//used to identify particle

  // counts the how many objects have been created and is used for the ID
  //flag that can be sat to true when particle is meant to be split
  bool m_split;
  // The links are stored by their index number in the particle system vector
  // I decided to not use pointers as the memory adress might change
  // we can think about a diffrent way of storing them later possibly using ID's looke at shared pointers as well
  std::vector<int> m_linkedParticles;
};

#endif // LINKEDPARTICLE_H
