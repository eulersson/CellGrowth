
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
                               qreal _z):Particle(_x,_y,_z)
{
   //qDebug("Linked Particle constructor passing in positions: %f,%f,%f", _x, _y, _z);
}


LinkedParticle::LinkedParticle(qreal _x,
                               qreal _y,
                               qreal _z,
                               std::vector<unsigned int> _linkedParticles): Particle(_x,_y,_z,_linkedParticles)
{

  //qDebug("Linked Particle constructor passing in positions: %f,%f,%f and a list of"
         //"particles", _x, _y, _z);

}



// All the force calculation should happen in here
void LinkedParticle::calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _averageDistance, unsigned int _particleCount, QVector3D _lightPos, int _cohesionFactor, int _springFactor)
{

  //  EQUIDISTANCE
  //  have found average distance away from centre
  QVector3D distance = _particleCentre - m_pos;
  QVector3D m_averageDistance = _averageDistance;
  if(_particleCount>100)
  {
    m_averageDistance*=1.5;
  }
  else if(_particleCount>10)
  {
    m_averageDistance*=1.2;
  }
  QVector3D fabsDistance;
  fabsDistance.setX(fabs (distance.x()));
  fabsDistance.setY(fabs (distance.y()));
  fabsDistance.setZ(fabs (distance.z()));

  if (fabsDistance.x() < m_averageDistance.x()
      && fabsDistance.y() < m_averageDistance.y()
      && fabsDistance.z() < m_averageDistance.z())
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
  QVector3D cohesion = distance;
  float cohesionLength = cohesion.length();
  float cohesionDist = m_size+(cohesionLength/2);
  if(cohesionLength>=m_size*2)
  {
    m_vel/=1.1;
  }

  cohesion.normalize();
  cohesion*=(cohesionDist/(_cohesionFactor*3.3f));
  m_vel+=cohesion;
  //end of cohere

  QVector3D connectionCentre;
  QVector3D planar;

  unsigned int connectionCount = getConnectionCount(); //gets number of connected particles

  std::vector<QVector3D> linkPosition;
  getPosFromConnections(linkPosition, _particleList);

//  PLANAR
//  Moves a particle to the average position of it's linked neighbours
//  for(unsigned int i=0; i<connectionCount; i++)
//  {
//    connectionCentre += linkPosition[i];
//  }
//  connectionCentre = connectionCentre/connectionCount;
//  planar = connectionCentre - m_pos;

//  float planarLength = planar.length();
//  float planarDist = m_size+(planarLength/2);
//  planar.normalize();
//  planar*=(planarDist/(_cohesionFactor*1.3f));
//  m_vel+=planar;
  //end of planar

  spring(_particleList, _springFactor);

  calculateUnlinked(_particleList, _cohesionFactor);


  //lightAttract(_particleList, _lightPos);
  //getHitParticles(_particleList, _lightPos);
  getNearestParticle(_particleList, _lightPos);

  particleLife++;

  //PARTICLELIFE

//  if (particleLife >= 100)
//  {
//    m_vel.setX(0);
//    m_vel.setY(0);
//    m_vel.setZ(0);
//  }

}

void LinkedParticle::spring(std::vector<std::unique_ptr<Particle>> &_particleList, int _springFactor)
{

  QVector3D spring;
  QVector3D hold;
  QVector3D linkedPos;
  std::vector<unsigned int> connectedParticles; //IDs of all the linked particles

  getConnectionsID(connectedParticles);

  //SPRING AND HOLD #2
//  for(unsigned int i=0; i<connectedParticles.size(); i++)
//  {
//    linkedPos = _particleList[i]->getPosition();
//    spring = linkedPos - m_pos;
//    float springLength = spring.length();
//    if(springLength>=m_size*2)
//    {
//      float springDist = m_size-(springLength/2);
//      spring.normalize();
//      spring*=(springDist/(_springFactor*3.3f));
//      m_vel+=spring;
//    }

//    hold = m_pos - linkedPos;
//    float holdLength = hold.length();
//    if(holdLength<=m_size)
//    {
//      float holdDist = m_size-(holdLength/2);
//      hold.normalize();
//      hold*=holdDist;
//      m_vel+=hold;
//    }
//  }

//  float distanceY = 0;
//  float distanceX = 0;
//  float distanceZ = 0;

//  unsigned int connectionCount = getConnectionCount(); //gets number of connected particles

//  std::vector<QVector3D> linkPosition;
//  getPosFromConnections(linkPosition, _particleList);

  //SPRING AND HOLD #2
//  for(unsigned int i=0; i<connectionCount; i++)
//  {
//   distanceY = linkPosition[i].y() - m_pos.y();
//   if(distanceY < m_size && distanceY > (-(m_size)))
//   {
//       QVector3D spring;
//       spring.setY(spring.y() - distanceY);
//       float factor = 10/fabs (distanceY);
//       spring/=10;
//       m_vel += spring;
//   }
//   else
//   {
//     float velY = m_vel.y();
//     velY /= 1.5;
//     m_vel.setY(velY);
//   }

//   distanceX = linkPosition[i].x() - m_pos.x();
//   if(distanceX < m_size && distanceX > (-(m_size)))
//   {
//       QVector3D spring;
//       spring.setX(spring.x() - distanceX);
//       spring/=10;
//       m_vel += spring;
//   }

//   else
//   {
//     float velX = m_vel.x();
//     velX /= 1.5;
//     m_vel.setX(velX);
//   }

//   distanceZ = linkPosition[i].z() - m_pos.z();
//   if(distanceZ < m_size && distanceZ > (-(m_size)))
//   {
//       QVector3D spring;
//       spring.setZ(spring.z() - distanceZ);
//       spring/=10;
//       m_vel += spring;
//   }
//   else
//   {
//     float velZ = m_vel.z();
//     velZ /= 1.5;
//     m_vel.setZ(velZ);
//   }

//  }

//  //HOLD
//  //sends particles that are too far away towards each other

// for(unsigned int i=0; i<connectionCount; i++)
// {
//   distanceY = linkPosition[i].y() - m_pos.y();
//   if(distanceY > m_size || distanceY < (-(m_size)))
//   {
//       QVector3D hold;
//       hold.setY(distanceY);
//       float factor = 500/fabs (distanceY);
//       if (distanceY > 1.5 || distanceY <(-1.5))
//       {
//         hold/=(factor/2);
//         m_vel+=hold;

//       }
//       else
//       {
//         hold/=factor;
//         m_vel+=hold;
//       }
//   }
//   else
//   {
//     float velY = m_vel.y();
//     velY /= 1.5;
//     m_vel.setY(velY);
//   }

//   distanceX = linkPosition[i].x() - m_pos.x();
//   if(distanceX > m_size || distanceX < (-(m_size)))
//   {
//       QVector3D hold;
//       hold.setX(distanceX);
//       float factor = 500/fabs (distanceX);
//       if (distanceX > 1.5 || distanceX <(-1.5))
//       {
//         hold/=(factor/2);
//         m_vel+=hold;

//       }
//       else
//       {
//         hold/=factor;
//         m_vel+=hold;
//       }
//   }

//   else
//   {
//     float velX = m_vel.x();
//     velX /= 1.5;
//     m_vel.setX(velX);
//   }

//   distanceZ = linkPosition[i].z() - m_pos.z();
//   if(distanceZ > m_size || distanceZ < (-(m_size)))
//   {
//       QVector3D hold;
//       hold.setZ(distanceZ);
//       float factor = 500/fabs (distanceZ);
//       if (distanceZ > 1.5 || distanceZ <(-1.5))
//       {
//         hold/=(factor/2);
//         m_vel+=hold;

//       }
//       else
//       {
//         hold/=factor;
//         m_vel+=hold;
//       }

//   }
//   else
//   {
//     float velZ = m_vel.z();
//     velZ /= 1.5;
//    m_vel.setZ(velZ);
//   }
// }
}

void LinkedParticle::calculateUnlinked(std::vector<std::unique_ptr<Particle>> &_particleList, int _cohesionFactor)
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
std::vector<unsigned int> LinkedParticle::getHitParticles(std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _lightPos)
{
  //Finds the particles within the point light radius so that they may be split
  std::vector<unsigned int> m_hitParticles;
  QVector3D lightDist;
  //std::cout<<"lightDist: "<<lightDist.x()<<std::endl;

  for(unsigned int i=0; i<=_particleList.size()-1; i++)
  {
    lightDist = (_particleList[i]->getPosition()) - _lightPos;
    if((lightDist.x()<=1 && lightDist.x() >= -1)
            && (lightDist.y()<=1 && lightDist.y() >= -1)
            && (lightDist.z()<=1 && lightDist.z() >= -1))
    {
      uint hitParticle = _particleList[i]->getID();
      //std::cout<<"hitParticle:"<<hitParticle<<std::endl;
      m_hitParticles.push_back(hitParticle);
    }
  }
  //std::cout<<"m_hitParticles.size(): "<<m_hitParticles.size()<<std::endl;
  return m_hitParticles;
}

unsigned int LinkedParticle::getNearestParticle(std::vector<std::unique_ptr<Particle>> &_particleList, QVector3D _lightPos)
{
  //Finds the particles within the point light radius so that they may be split
  //std::vector<unsigned int> m_nearestParticles;
  std::vector<float> m_lightDistances;

  for (unsigned int i=0; i<=_particleList.size()-1; i++)
  {
    QVector3D lightDist = (_particleList[i]->getPosition() - _lightPos);
    m_lightDistances.push_back(lightDist.length());
  }

  std::vector<float>::iterator minElement=std::min_element (std::begin(m_lightDistances), std::end(m_lightDistances));
  unsigned int minElementIndex = std::distance(std::begin(m_lightDistances), minElement);
  //std::cout<<"minElementIndex:"<<minElementIndex<<std::endl;

//  std::vector<float>::iterator minElement2;
//  std::vector<float>::iterator minElement3;
//  unsigned int minElementIndex2;
//  unsigned int minElementIndex3;

//  if (minElementIndex!=0)
//  {
//     minElement2 = std::min_element (std::begin(m_lightDistances), minElement);
//     minElementIndex2 = std::distance(std::begin(m_lightDistances), minElement2);
//     //std::cout<<"minElementIndex2:"<<minElementIndex2<<std::endl;
//  }
//  if (minElementIndex!=(m_lightDistances.size()-1))
//  {
//     minElement3 = std::min_element (minElement+1, std::end(m_lightDistances));
//     minElementIndex3 = std::distance(std::begin(m_lightDistances), minElement3);
//     //std::cout<<"minElementIndex3:"<<minElementIndex3<<std::endl;
//  }

//  m_nearestParticles.push_back(minElementIndex);

//  if (minElementIndex=0)
//  {
//    m_nearestParticles.push_back(minElementIndex3);
//  }

//  else if (minElementIndex=(m_lightDistances.size()-1))
//  {
//    m_nearestParticles.push_back(minElementIndex2);
//  }

//  else
//  {
//    if (m_lightDistances[minElementIndex2] < m_lightDistances[minElementIndex3])
//    {
//      m_nearestParticles.push_back(minElementIndex2);
//    }
//    else
//    {
//      m_nearestParticles.push_back(minElementIndex3);
//    }

//  }

  return minElementIndex;
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

void LinkedParticle::split(std::vector<std::unique_ptr<Particle>> &_particleList,std::mt19937_64 _gen)
{
 // hitParticles = getHitParticle(_particleList, _lightPos);
 // std::cout<<"m_hitParticles.size(): "<<m_hitParticles.size()<<std::endl;

  // Sanity check
  if(m_connectedParticles.size() < 2)
    return;

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
  std::cout<<"splitting between particles "<<a<<" and "<<b<<std::endl;
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

  _particleList.push_back(std::unique_ptr<Particle>(new LinkedParticle(x,y,z,relinkList)));

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
    _particleList[relinkList[i]]->connect(newPartID);

  }

  //link both, parent and child, to each other
  m_connectedParticles=keepList;


  connect(newPartID);

  m_foodLevel=0;

//  QVector3D particleCentre;
//  QVector3D averageDistance;
//  std::vector<unsigned int> returnList;
//  calculate(particleCentre, _particleList, averageDistance, returnList);

}
