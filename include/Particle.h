#ifndef PARTICLE_H
#define PARTICLE_H

// Native
#include<memory>

// Qt
#include<QVector3D>


//--------------------------------------------------------------------------------------------------------------------
/// @file Particle.h
/// @brief this class is a base particle class providing common methods and attributes
/// @author Carola Gille, Ramon B *i don't know how to spell your last name*
/// @version 1.0
/// @date 14/02/17



class Particle
{
public:

  /// @brief default constructor placing particle at the origin
  Particle();

  /// @brief custom constructor allowing user input for position
  /// @param[in] _x x position of the particle
  /// @param[in] _y y position of the particle
  /// @param[in] _z z position of the particle
  Particle(qreal _x,
           qreal _y,
           qreal _z);

  /// @brief custom constructor allowing user input for position
  /// @param[in] _x x position of the particle
  /// @param[in] _y y position of the particle
  /// @param[in] _z z position of the particle
  /// @param[in] _connectedParticles list of particle IDs to be connected to this particle
  Particle(qreal _x
           ,qreal _y,
           qreal _z,
           std::vector<int> _connectedParticles);

  /// @brief adds the velocity to the position, could be overwritten if inherited
  void advance();//overwrite this function in inherited class

  /// @brief Particle dependent function for splitting, needs to be overwritten if inherited
  void split();//overwrite function in inheritade class

  /// @brief tests if the particle has reached its food treshold and therefor needs to be split
  void testForSplit();

  /// @brief returns Particles position
  /// @param[in] _pos will hold the particles position
  void getPos(QVector3D &_pos);

  /// @brief sets the Particles position
  /// @param[in] _x new x position of the Particle
  /// @param[in] _y new y position of the Particle
  /// @param[in] _z new z position of the Particle
  void setPos(qreal _x,
              qreal _y,
              qreal _z);

  /// @brief adds a particle Id to the current particle, is used to create a "connection" between to particles and should be used by another particle to create the connection
  /// @param[in] _ID Id of the particle that is to be connected to the current particle
  void connect(int _ID);

  /// @brief deletes an Id from the conncetion List and so breaks the connection between two particles
  /// @param[in] _ID Id to be deleted
  void deleteConnection(int _ID);

  ///@brief returns the number of connections
  /// @param[out] the number of connections
  int getConnectionCount();

  /// @brief returns the particles ID
  /// @param[out] ID of the particle
  int getID();

  /// @brief returns a list including all ID that are connected to the particle
  /// @param[in] _returnList will hold the Id's
  void getConnectionsID(std::vector<int> &_returnList);

  /// @brief returns a list with all positions of the particles connections
  /// @param[in] _linkPos return list with all positions
  /// @param[in] _particleList list from the particle system holding all existing particles
  void getPosFromConnections(
      std::vector<QVector3D> &_linkPos,
      std::vector<std::unique_ptr<Particle>> &_particleList);

  /// @brief returns the index of the particle in the particle systhem
  /// @param[in] list from the particle system holding all existing particles
  /// @param[out] index number of the the particle in the particle system
  int getPosInPS(std::vector<std::unique_ptr<Particle> > &_particleList);
private:

  /// @brief particles position
  QVector3D m_pos;

  /// @brief velocity that is used to move the particle each frame
  QVector3D m_vel;

  /// @brief counts the number of objects created from this class
  static int m_ID_counter;

  /// @brief unique ID of particle used for storing connections
  int m_ID;

  /// @brief Particle size
  float m_size;

  /// @brief flag that is set when the particle needs to be split
  bool m_split;

  /// @brief holds Id of all particles connected to this particle
  std::vector<int> m_connectedParticles;//changed name from linked particles

  /// @brief food level, increses when the particle is hit by light
  unsigned int m_foodLevel;

  /// @brief feed threshold, when reached particle is split
  unsigned int m_foodTreshold;

};



#endif // PARTICLE_H
