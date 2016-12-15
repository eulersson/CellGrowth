#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

#include<QVector3D>
#include<memory>
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

  // Can be used by other particles to link a particle with this particle
  void link(int _ID);

  //deletes a linked particle from the linked particle list and so destroys the link
  void deleteLink(int _ID);
  //queries the ID of the particle

  //Computes all the relinking and creates a new particle
  void split(std::vector<std::unique_ptr<LinkedParticle> > &_particleList);

  int getLinkCount();
  //returns ID
  int getID();

  //returns links from linkedParticleList , list contains ID's not positions
  // could be changed to return positions
  void getLinks(std::vector<int> &_returnList);
  //returns position of links
  void getPosFromLinks(std::vector<QVector3D> &_linkPos, std::vector<std::unique_ptr<LinkedParticle>> &_particleList);
  // gets the position in the particle system list
  int getPosInPS(std::vector<std::unique_ptr<LinkedParticle> > &_particleList);

private:
  //private methode used by the split methode to sort particles depending on what side of a plane they are on
  //returns the distance to the plane
  int planeSorting(QVector3D _normal,QVector3D _planePoint,QVector3D _testPoint);
  QVector3D m_pos;
  QVector3D m_vel;

  //used to keep an object count and give unique Id's to each particle
  static int m_ID_counter;

  //used to identify particle and is stored in the the linked particle list of other particles
  //unique to each particle
  int m_ID;

  //flag that can be sat to true when particle is meant to be split
  bool m_split;

  // The links are stored by their ID number
  // I decided to not use pointers as the memory adress might change
  // we can think about a diffrent way of storing them later
  std::vector<int> m_linkedParticles;

};

#endif // LINKEDPARTICLE_H
