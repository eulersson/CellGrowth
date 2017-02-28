#include "LinkedParticle.h"
#include <iostream>
LinkedParticle::LinkedParticle():Particle()
{
  qDebug("Linked Particle default constructor.");
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
    //     "particles", _x, _y, _z);

}



// All the force calculation should happen in here
void LinkedParticle::calculate(QVector3D _newParticleCentre, std::vector<QVector3D> m_listOfPositions)
{

  //COHERE
  unsigned int speed = 1000;
  QVector3D cohesion = _newParticleCentre - m_pos;
  //std::cout<<"Distance:"<<cohesion.x()<<" "<<cohesion.y()<<" "<<cohesion.z()<<std::endl;
  //std::cout<<"Velocity: "<<m_vel.x()<<" "<<m_vel.y()<<" "<<m_vel.z()<<std::endl;
  if((_newParticleCentre.x()-m_pos.x() <= 1)
           && (_newParticleCentre.y()-m_pos.y() <= 1)
           && (_newParticleCentre.z()-m_pos.z() <= 1))
  {
      m_vel/=1.1;
  }

    cohesion/=speed;

    m_vel += cohesion;

    //SPRING
    QVector3D spring;
    spring.setX(0);
    spring.setY(0);
    spring.setZ(0);

    float distanceX = 0;
    float distanceY = 0;
    float distanceZ = 0;

    //std::cout<<"SIZE:"<<m_listOfPositions.size()<<std::endl;

    for(unsigned int i=0; i<m_listOfPositions.size(); i++)
    {
      std::cout<<"listOfPositions"<<i<<": "<<m_listOfPositions[i].x()<<" "<<m_listOfPositions[i].y()<<" "<<m_listOfPositions[i].z()<<std::endl;
    }

//    for(unsigned int i=0; i<m_listOfPositions.size(); i++)
//    {
//      std::cout<<"m_ID: "<<m_ID<<std::endl;
//    }

    //Using i and j to compare particle distances in space. Eg, i=particleA and j=particleB
    for (unsigned int i=0; i<m_listOfPositions.size(); i++)
    {
      for (unsigned int j=0; j<m_listOfPositions.size(); j++)
        {
          if (i != j)
          {
            distanceY = (m_listOfPositions[j].y()) - (m_listOfPositions[i].y());
            if (distanceY < 0.3 && distanceY >(-0.3) )
            {
              if(m_ID == i)
              {
                spring.setY(spring.y() - distanceY);
                spring/=50;
                m_vel += spring;
              }
//              if(m_ID == j)
//              {
//                spring.setY(spring.y() - distanceY);
//                //spring/=10;
//                m_vel += spring;
//              }
            }

            distanceX = (m_listOfPositions[j].x()) - (m_listOfPositions[i].x());
            if (distanceX < 0.3 && distanceX >(-0.3) )
            {
              if(m_ID == i)
              {
                spring.setX(spring.x() - distanceX);
                spring/=50;
                m_vel += spring;
              }
//              if(m_ID == j)
//              {
//                spring.setX(spring.x() + distanceX);
//                spring/=100;
//                m_vel += spring;
//              }
            }

            distanceZ = (m_listOfPositions[j].y()) - (m_listOfPositions[i].y());
            if (distanceZ < 0.3 && distanceZ >(-0.3) )
            {
              if(m_ID == i)
              {
                spring.setZ(spring.z() - distanceZ);
                spring/=50;
                m_vel += spring;
              }
//              if(m_ID == j)
//              {
//                spring.setZ(spring.z() + distanceZ);
//                spring/=1000;
//                m_vel += spring;
//              }
            }
          }
        }
    }

}


int LinkedParticle::planeSorting(QVector3D _normal, QVector3D _planePoint, QVector3D _testPoint)
{
  int d=_normal.x()*_planePoint.x()+_normal.y()*_planePoint.y()+_normal.z()*_planePoint.z();
  int r=_normal.x()*_testPoint.x()+_normal.y()*_testPoint.y()+_normal.z()*_testPoint.z()-d;
  return r;
}

void LinkedParticle::split(std::vector<std::unique_ptr<Particle>> &_particleList)
{

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int> distribution(1, m_connectedParticles.size() - 1);

  //holds all ID's of the partciles that are keept by the current particle
  std::vector<unsigned int> keepList;

  //holds all the ID's of the particles that are linked to the new particle
  std::vector<unsigned int> relinkList;

  // holds the positions of the linked particles
  std::vector<QVector3D> linkPosition;

  getPosFromConnections(linkPosition, _particleList);


  //pick two random particles out of the particle list
  //saving index number of it in list not Id or Pos

  unsigned int a=0;

  unsigned int b=distribution(gen);

  if (a == b)
  {
    std::cout << "WARNING you can't split between a particle and itself" << std::endl;
  }

  QVector3D normal = QVector3D::normal(linkPosition[a], linkPosition[b]);

  //filling two arrays with links based on there position relative to the plane created by the two first particles
  for(unsigned int i=0;i<m_connectedParticles.size();i++)
  {
    if(i == a || i == b)
    {
      //the two plane particles are linked too both old and new particle
      keepList.push_back(m_connectedParticles[i]);
      relinkList.push_back(m_connectedParticles[i]);

    }
    else
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

  //


  normal.normalize();
  //create new particle
  qreal x=m_pos.x()+normal.x()/20;
  qreal y=m_pos.y()+normal.y()/20;
  qreal z=m_pos.z()+normal.z()/20;



  relinkList.push_back(m_ID);
  //creating new particle


  _particleList.push_back(std::unique_ptr<Particle>(new LinkedParticle(x,y,z,relinkList)));

  //get the new particles ID

  int newPartID=_particleList[_particleList.size()-1]->getID();

  //link all the particles to the new particle
  for(unsigned int i=0;i<relinkList.size();i++)
  {
    for(unsigned int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==relinkList[i])
      {
        _particleList[j]->connect(newPartID);
        break;
      }
    }
  }

  //link both, parent and child, to each other
  m_connectedParticles=keepList;

  //delete links from to old particles

  for(unsigned int i=0;i<relinkList.size();i++)
  {
    //find correct linked particle in particle list
    for(unsigned int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==relinkList[i])
      {
        _particleList[j]->deleteConnection(m_ID);
        break;
      }
    }
  }
  connect(newPartID);

  m_foodLevel=0;
}









