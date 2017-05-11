
////////////////////////////////////////////////////////////////////////////////
/// @file LinkedParticle.cpp
/// @author Carola Gille
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include <random>
#include "LinkedParticle.h"
#include <iostream>
LinkedParticle::LinkedParticle():Particle()
{
  //qDebug("Linked Particle default constructor.");
}


// Specify the location where we want to place it
LinkedParticle::LinkedParticle(qreal _x,
                               qreal _y,
                               qreal _z, float _size):Particle(_x,_y,_z, _size)
{
   //qDebug("Linked Particle constructor passing in positions: %f,%f,%f", _x, _y, _z);
}


LinkedParticle::LinkedParticle(qreal _x,
                               qreal _y,
                               qreal _z,
                               std::vector<unsigned int> _linkedParticles, float _size): Particle(_x,_y,_z,_linkedParticles, _size)
{

  //qDebug("Linked Particle constructor passing in positions: %f,%f,%f and a list of"
         //"particles", _x, _y, _z);

}


void LinkedParticle::calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _averageDistance, unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _localCohesionFactor, bool _particleDeath)
{


  unsigned int connectionCount = getConnectionCount();
  std::vector<QVector3D> linkPosition;


    // EQUIDISTANCE
    // Calculates average distance from centre
    // Encourages particles towards this distance from centre
    // Stops the particles from wanting to get too close to the middle

    QVector3D distance = _particleCentre - m_pos;
    //std::cout<<"averageDistance"<<_averageDistance.x()<<_averageDistance.y()<<_averageDistance.z()<<std::endl;
    QVector3D m_averageDistance = _averageDistance;

    if (distance.lengthSquared()<m_averageDistance.lengthSquared())
    {
        QVector3D sendAway = -(distance);
        sendAway/=100;
        m_vel+=sendAway;
    }
    else
    {
      m_vel/=1.5;
    }


  //COHESION
  //Calculates cohesion based on all particles
  //Sends particles towards particle centre based on distance from centre
  QVector3D cohesion = _particleCentre - m_pos;
  float cohesionLength = cohesion.length();
  float cohesionDist = m_size+(cohesionLength/2);
  if(cohesionLength>=m_size*2)
  {
    m_vel/=1.1;
  }

  cohesion.normalize();
  cohesion*=(cohesionDist/(_cohesionFactor*3.3f));
  m_vel+=cohesion;


  //LOCAL COHESION
  //Calculates cohesion based on particles links
  //Finds the centre of the linked particles
  //Influences all particles towards that centre
  QVector3D connectionCentre;
  //unsigned int connectionCount = getConnectionCount();
  //std::vector<QVector3D> linkPosition;
  getPosFromConnections(linkPosition, _particleList);

  for(unsigned int i=0; i<connectionCount; i++)
  {
    connectionCentre += linkPosition[i];
  }
  connectionCentre = connectionCentre/connectionCount;
  QVector3D localCohesion = connectionCentre - m_pos;

  float localCohesionLength = localCohesion.length();
  float localCohesionDist = m_size+(localCohesionLength/2);
  if(localCohesionLength>=m_size*2)
  {
    m_vel/=1.1;
  }

  localCohesion.normalize();
  localCohesion*=(localCohesionDist/(_localCohesionFactor));
  m_vel+=localCohesion;


  //CALCULATE UNLINKED
  //Makes a call to calculate unlinked function

  calculateUnlinked(_particleList);


  //PARTICLELIFE
  //Determines how long the particle has been alive
  //Sets velocity to 0 if particle has been alive too long

  if (_particleDeath == true)
  {
  particleLife++;
  for (unsigned int i=0; i<connectionCount;i++)
  {
    QVector3D distanceFromLinkedParticles = linkPosition[i] - m_pos;

    if (particleLife >= 200 && distanceFromLinkedParticles.length()<=(m_size*2))
    {
      m_vel.setX(0);
      m_vel.setY(0);
      m_vel.setZ(0);
    }
  }
  };

}


void LinkedParticle::calculateUnlinked(std::vector<std::unique_ptr<Particle>> &_particleList)
{
  //REPULSE
  //Move the particles which aren't linked away from each other
  QVector3D repulse;
  QVector3D unlinkedPos;
  std::vector<unsigned int> allParticles; //IDs of all particles
  std::vector<unsigned int> notConnected; //IDs of all the unlinked particles
  std::vector<unsigned int> connectedParticles; //IDs of all the linked particles

  for(unsigned int i=0; i<m_ID_counter; i++)
  {
    allParticles.push_back(i);
    getConnectionsID(connectedParticles);
  }

  getConnectionsID(connectedParticles);

  std::sort(allParticles.begin(), allParticles.end());
  std::sort(connectedParticles.begin(), connectedParticles.end());

  //creates a vector of unlinked particles by finding all of the IDs in the allParticles list which aren't in the connectedParticles list
  std::set_difference(allParticles.begin(), allParticles.end(),
                      connectedParticles.begin(), connectedParticles.end(),
                      std::back_inserter(notConnected));

  for(unsigned int j=0; j<notConnected.size(); j++)
  {
    if(m_ID != notConnected[j])
    {
      unlinkedPos = _particleList[j]->getPosition();
      repulse = m_pos - unlinkedPos;
      float length = repulse.length();
      if(length<=m_size*2)
      {
        float distance = m_size-(length/2);
        repulse.normalize();
        repulse*=distance;
        m_vel+=repulse;
      }
    }
  }
}

void LinkedParticle::bulge(QVector3D _particleCentre)
{
  //BULGE
  //Finds the particles closest to the centre and move them outwards on a key press

  QVector3D distance = m_pos - _particleCentre;
  if(distance.x() <= m_size*2
                || distance.y() <= m_size*2
                || distance.z() <= m_size*2)
  {
    //distance/=10;

    m_vel += distance;
  }
}

void LinkedParticle::addFood(QVector3D _particleCentre)
{

  //FOOD LEVEL
  //Changes food level of random particles and sends them inwards

  if (m_foodLevel == true)
  {

    life++;
    QVector3D food = _particleCentre - m_pos;

    if(food.length() <= m_size*2)
    {
        m_vel/=1.1;

    }
    food/=4;
    m_vel += food;

    std::cout<<"life:"<<life<<std::endl;
    if (life >= 10)
    {
      m_foodLevel = false;
    }

  }
}

std::vector<unsigned int> LinkedParticle::getHitParticles(std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _lightPos)
{
  //Finds the particles within the point light radius so that they may be split
  std::vector<unsigned int> m_hitParticles;
  QVector3D lightDist;
  //std::cout<<"lightDist: "<<lightDist.x()<<std::endl;

  for(unsigned int i=0; i<=_particleList.size()-1; i++)
  {
    lightDist = (_particleList[i]->getPosition()) - _lightPos;
    std::cout<<"lightDistancelength:"<<lightDist.length()<<std::endl;
    if(lightDist.length()<4)
    {
      uint hitParticle = _particleList[i]->getID();
      //std::cout<<"hitParticle:"<<hitParticle<<std::endl;
      m_hitParticles.push_back(hitParticle);
    }
  }
  std::cout<<"m_hitParticles.size(): "<<m_hitParticles.size()<<std::endl;
  return m_hitParticles;
}

void LinkedParticle::lightAttract(std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _lightPos)
{

  QVector3D lightDist;
  lightDist = m_pos - _lightPos;

  if((lightDist.x()<=0.5)
          && (lightDist.y()<=0.5)
          && (lightDist.z()<=0.5))
  {
    lightDist = _lightPos - m_pos;
    lightDist /= 50;
    m_vel += lightDist;

  }

}

int LinkedParticle::planeSorting(QVector3D _normal, QVector3D _planePoint, QVector3D _testPoint)
{
  //sorts point depending on the position relative to a plane

  int d=_normal.x()*_planePoint.x()+_normal.y()*_planePoint.y()+_normal.z()*_planePoint.z();
  int r=_normal.x()*_testPoint.x()+_normal.y()*_testPoint.y()+_normal.z()*_testPoint.z()-d;
  return r;
}

bool LinkedParticle::split(std::vector<std::unique_ptr<Particle>> &_particleList,std::mt19937_64 _gen)
{
 // hitParticles = getHitParticle(_particleList, _lightPos);
 // std::cout<<"m_hitParticles.size(): "<<m_hitParticles.size()<<std::endl;

  // Sanity check
  if(m_connectedParticles.size() < 2)
  {
    std::cout<<"NOT ENOUGH PARTICLES"<<std::endl;
    return false;
  }
  std::uniform_int_distribution<int> distribution(1, m_connectedParticles.size());

  //holds all ID's of the particles that are kept by the current particle
  std::vector<unsigned int> keepList;

  //holds all the ID's of the particles that are linked to the new particle
  std::vector<unsigned int> relinkList;

  // holds the positions of the linked particles
  std::vector<QVector3D> linkPosition;

  getPosFromConnections(linkPosition, _particleList);

  //pick two random particles out of the particle list
  //saving index number of it in list not Id or Pos to
  //avoid searching the particle list for the particle again


  unsigned int a=0;

  unsigned int b=distribution(_gen) - 1;
  if(b == a)
  {
    while(b == a)
      b = distribution(_gen) - 1;
  }

  QVector3D normal = QVector3D::normal(linkPosition[a], linkPosition[b]);

  //filling two arrays with links based on there position relative to the plane created by the two first particles
  for(unsigned int i=0;i<m_connectedParticles.size();i++)
  {
    if(i != a && i != b)
    {
      int r=planeSorting(normal,linkPosition[a],linkPosition[i]);
      if(r<=0)
      {
        keepList.push_back(m_connectedParticles[i]);
      }
      else
      {
        relinkList.push_back(m_connectedParticles[i]);
      }
    }
  }

  normal.normalize();
  //create new particle
  qreal x=m_pos.x()+normal.x()*m_size;
  qreal y=m_pos.y()+normal.y()*m_size;
  qreal z=m_pos.z()+normal.z()*m_size;

  relinkList.push_back(m_ID);
  //creating new particle

  _particleList.push_back(std::unique_ptr<Particle>(new LinkedParticle(x,y,z,relinkList,m_size)));

  //get the new particles ID

  int newPartID=_particleList[_particleList.size()-1]->getID();

  //delete links from old particles


  for(unsigned int i=0;i<relinkList.size();i++)
  {
     _particleList[relinkList[i]]->deleteConnection(m_ID);
  }


  keepList.push_back(m_connectedParticles[a]);
  keepList.push_back(m_connectedParticles[b]);
  relinkList.push_back(m_connectedParticles[a]);
  relinkList.push_back(m_connectedParticles[b]);

  //link all the particles to the new particle
  for(unsigned int i=0;i<relinkList.size();i++)
  {
    _particleList[relinkList[i]]->connect(newPartID,_particleList);

  }

  //link both, parent and child, to each other
  m_connectedParticles=keepList;


  doubleConnect(newPartID,_particleList);


//  QVector3D particleCentre;
//  QVector3D averageDistance;
//  std::vector<unsigned int> returnList;
//  calculate(particleCentre, _particleList, averageDistance, returnList);
  return true;

}

void LinkedParticle::doubleConnect(unsigned int _ID, std::vector<std::unique_ptr<Particle> > &_particleList)
{
  m_connectedParticles.push_back(_ID);

  std::vector<unsigned int> connections;

  _particleList[_ID]->getConnectionsID(connections);

  for(unsigned int i=0;i<connections.size();i++)
  {
    if(connections[i]==m_ID)
    {return;}

  }
  _particleList[_ID]->connect(m_ID,_particleList);
}

