#ifndef LINKEDPARTICLE_H
#define LINKEDPARTICLE_H

// Native
#include<memory>

// Qt
#include<QVector3D>

class LinkedParticle
{
public:
  // Constructors
  LinkedParticle();
  LinkedParticle(qreal _x,qreal _y,qreal _z);
  LinkedParticle(qreal _x,qreal _y,qreal _z,std::vector<int> _linkedParticles);

  // Updates the position after forces are computed only called after all new
  // position for particles are calculated
  void advance();

  // Computes the forces that act on this particle
  void calculate();

  // Returns true if the particle is meant to be split
  bool testForSplit();

  // Writes the position to the QVector3D that is passed by reference
  void getPos(QVector3D &_vector);

  // Sets the current position of the linked particle
  void setPos(qreal _x,qreal _y,qreal _z);

  // Can be used by other particles to link a particle with this particle
  void link(int _ID);

  // Deletes a linked particle from the linked particle list, destroys the link
  void deleteLink(int _ID);

  // Computes all the relinking and creates a new particle
  void split(std::vector<std::unique_ptr<LinkedParticle> > &_particleList);

  // Gives the number of links to a particle
  int getLinkCount();

  // ID getter
  int getID();

  // Returns links from linkedParticleList, list contains ID's not positions
  // Could be changed to return positions
  void getLinks(std::vector<int> &_returnList);

  // Returns position of links
  void getPosFromLinks(
      std::vector<QVector3D> &_linkPos,
      std::vector<std::unique_ptr<LinkedParticle>> &_particleList);

  // Gets the position in the particle system list
  int getPosInPS(std::vector<std::unique_ptr<LinkedParticle> > &_particleList);

private:
  // Private method used by the split method to sort particles depending on
  // what side of a plane they are on returns the distance to the plane
  int planeSorting(QVector3D _normal,QVector3D _planePoint,QVector3D _testPoint);

  // Position of the particle
  QVector3D m_pos;

  // Velocity of the particle (will be calculated based on forces and will
  // update the current position)
  QVector3D m_vel;

  // Used to keep an global count count and give unique ID's to each particle
  static int m_ID_counter;

  // Used to identify particle and is stored in the the linked particle list of
  // other particles. Unique to each particle.
  int m_ID;

  // Flag that can be sat to true when particle is meant to be split
  bool m_split;

  // The links are stored by their ID number
  // I decided to not use pointers as the memory address might change
  // we can think about a diffrent way of storing them later
  std::vector<int> m_linkedParticles;

  // This will keep track of the food level that is increased by the sun
  unsigned int m_foodLevel;

  // This variable could be made different to emulate different cell types
  unsigned int m_foodTreshold;
};

#endif // LINKEDPARTICLE_H
