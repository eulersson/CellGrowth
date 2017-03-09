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
        // "particles", _x, _y, _z);

}



// All the force calculation should happen in here
void LinkedParticle::calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle>> &_particleList)
{
    //COHERE
    QVector3D distance = _particleCentre - m_pos;

    QVector3D cohesion = distance;

    if((distance.x() <= m_size)
           && (distance.y() <= m_size)
           && (distance.z() <= m_size))
    {
        m_vel/=1.1;
    }

    cohesion/=3000;
    m_vel += cohesion;
    //end of cohere

    //HOLD
    //coheres linked particles together
    //SPRING
    //pushes particles away from each other

    QVector3D hold;
    QVector3D spring;
    QVector3D connectionCentre;
    QVector3D planar;
    float distanceY = 0;
    float distanceX = 0;
    float distanceZ = 0;

    unsigned int connectionCount = getConnectionCount(); //gets number of connected particles)

    std::vector<QVector3D> linkPosition;
    getPosFromConnections(linkPosition, _particleList);

    for(unsigned int i=0; i<connectionCount; i++)
    {
       connectionCentre += linkPosition[i];

       hold = linkPosition[i] - m_pos;

       if (hold.x() <= m_size
                 || hold.y() <= m_size
                 || hold.z() <= m_size)
       {
          m_vel/=1.1;
       }

       hold/=5000;
       m_vel+=hold;

       distanceY = linkPosition[i].y() - m_pos.y();
       if(distanceY < m_size && distanceY > (-(m_size)))
       {
           spring.setY(spring.y() - distanceY);
           spring/=30;
           m_vel += spring;
       }

       distanceX = linkPosition[i].x() - m_pos.x();
       if(distanceX < m_size && distanceX > (-(m_size)))
       {
           spring.setX(spring.x() - distanceX);
           spring/=30;
           m_vel += spring;
       }

       distanceZ = linkPosition[i].z() - m_pos.z();
       if(distanceZ < m_size && distanceZ > (-(m_size)))
       {
           spring.setZ(spring.z() - distanceZ);
           spring/=30;
           m_vel += spring;
       }
    }

    //PLANAR
    //Moves a particle to the average position of it's linked neighbours
    connectionCentre = connectionCentre/connectionCount;
    planar = connectionCentre - m_pos;

    if((planar.x() <= m_size)
           && (planar.y() <= m_size)
           && (planar.z() <= m_size))
    {
        m_vel/=1.1;
    }

    planar/=1000;
    m_vel += planar;

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









