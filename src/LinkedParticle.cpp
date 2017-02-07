#include "LinkedParticle.h"
#include <iostream>
#include <random>


// Default constructor puts it at origin
LinkedParticle::LinkedParticle()
  : m_ID(m_ID_counter++)
  , m_foodLevel(0)
  , m_foodTreshold(0)
  , m_pos(QVector3D(0,0,0))
  , m_particleTreshold(3)
  , m_size(0.1)
{
  std::cout << "Calling Linked Particle Default Constructor" << std::endl;
}


// Specify the location where we want to place it
LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z)
  : m_ID(m_ID_counter++)
  , m_foodLevel(0)
  , m_foodTreshold(100)
  , m_pos(QVector3D(_x, _y, _z))
  , m_particleTreshold(3)
  , m_size(0.1)
{
  std::cout
      << "LinkedParticle Constructor with positions "
      << _x << ", " << _y << ", " << _z
      << std::endl;
}


LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z,std::vector<int> _linkedParticles)
  : m_ID(m_ID_counter++)
  , m_foodLevel(0)
  , m_foodTreshold(100)
  , m_pos(QVector3D(_x, _y, _z))
  , m_particleTreshold(3)
  , m_size(0.1)
{

  std::cout << "Calling Linked Particle Costume Constructor" << std::endl;
  m_linkedParticles = _linkedParticles;
  //if (m_linkedParticles.size() < 3)
//  {
//    std::cout << "Warning not enough links in Particle when Constructed" << std::endl;
//  }
}


// After force calculations are done, we advect the position
void LinkedParticle::advance()
{
  // Call calculate() to do all the maths to update the forces and update the
  // velocity. When we are done:
  m_pos += m_vel;
}


// All the force calculation should happen in here
void LinkedParticle::calculate()
{
}

//alternative calculate function
void LinkedParticle::split(QVector3D _lightPosition,std::vector<std::unique_ptr<LinkedParticle>> &_particleList)
{

  std::random_device rd;
  std::mt19937_64 gen(rd());

  //triggered by reaching the threshold
  //check length of linked list to see if the max particle treshold is reached
  if(m_linkedParticles.size()>=m_particleTreshold)
  {return;}

  //create new particle

    //creating list for new particlen including mother particle
    std::vector<int> newLinkedParticles;
    //appends mother ID
    newLinkedParticles.push_back(m_ID);

    std::uniform_real_distribution<float> distributionX(m_pos[0]+0.001,_lightPosition[0]);
    std::uniform_real_distribution<float> distributionY(m_pos[1]+0.001,_lightPosition[1]);
    std::uniform_real_distribution<float> distributionZ(m_pos[2]+0.001,_lightPosition[2]);






  //place new particle on side in  direction of light

    //loop
    //choose random point on side of light
    float x;
    float y;
    float z;
    QVector3D pos;
    do{



    x= distributionX(gen);
    y= distributionY(gen);
    z= distributionZ(gen);


    //calculate vector
    QVector3D direction;
    direction[0]=m_pos[0]-x;
    direction[1]=m_pos[1]-y;
    direction[2]=m_pos[2]-z;

    //place new particle in direction of vector mutilplied by size of particle
    direction.normalize();

    direction[0]*=m_size;
    direction[1]*=m_size;
    direction[2]*=m_size;
    x=m_pos[0]+direction[0];
    y=m_pos[1]+direction[1];
    z=m_pos[2]+direction[2];
    //check for collision

    pos[0]=x;
    pos[1]=y;
    pos[2]=z;


    }
    while(collision(2,pos,_particleList));



    // create new particle and add to particle list
    //qDebug("%f ,// %f ,// %f", x, y, z);

    _particleList.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle(x,y,z,newLinkedParticles)));
    // add particle to links in original particle
    int new_ID = _particleList[_particleList.size()-1 ]->getID();
    m_linkedParticles.push_back(new_ID);


}

bool LinkedParticle::testForSplit()
{
  if (m_foodLevel >= m_foodTreshold)
  {
    return true;
  }
  else
  {
    return false;
  }
}


// For modifying the position of the particle
void LinkedParticle::setPos(qreal _x, qreal _y, qreal _z)
{
  m_pos.setX(_x);
  m_pos.setY(_y);
  m_pos.setZ(_z);
}


// Position getter
void LinkedParticle::getPos(QVector3D &_vector)
{
  _vector.setX(m_pos.x());
  _vector.setY(m_pos.y());
  _vector.setZ(m_pos.z());
}


// Adds linkedParticles ID to the linked Particle List
void LinkedParticle::link(int _ID)
{
  m_linkedParticles.push_back(_ID);
}


// Iterates through linked particles. List and erases the input ID
void LinkedParticle::deleteLink(int _ID)
{
  for (unsigned int i = 0; i < m_linkedParticles.size(); i++)
  {
    if (m_linkedParticles[i] == _ID)
    {
      m_linkedParticles.erase(m_linkedParticles.begin() + i);
    }
    break;
  }

  //if(m_linkedParticles.size()<3)
    //std::cout<<"Warning not enough links in Particle"<<std::endl;
}


// ID getter
int LinkedParticle::getID()
{

 return m_ID;
}


// Links getter (in form of IDs)
void LinkedParticle::getLinks(std::vector<int> &_returnList)
{
  _returnList=m_linkedParticles;
}


// Returns the distance of a plane defined by a normal and a point
int LinkedParticle::planeSorting(QVector3D _normal, QVector3D _planePoint, QVector3D _testPoint)
{
  int d=_normal.x()*_planePoint.x()+_normal.y()*_planePoint.y()+_normal.z()*_planePoint.z();
  int r=_normal.x()*_testPoint.x()+_normal.y()*_testPoint.y()+_normal.z()*_testPoint.z()-d;
  return r;
}


// Splits
void LinkedParticle::split(std::vector<std::unique_ptr<LinkedParticle>> &_particleList)
{
  std::cout << "Splitting particle " << m_ID << std::endl;

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int> distribution(1, m_linkedParticles.size() - 1);

  //holds all ID's of the partciles that are keept by the current particle
  std::vector<int> keepList;

  //holds all the ID's of the particles that are linked to the new particle
  std::vector<int> relinkList;

  // holds the positions of the linked particles
  std::vector<QVector3D> linkPosition;

  getPosFromLinks(linkPosition, _particleList);


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
  for(unsigned int i=0;i<m_linkedParticles.size();i++)
  {
    if(i == a || i == b)
    {
      //the two plane particles are linked too both old and new particle
      keepList.push_back(m_linkedParticles[i]);
      relinkList.push_back(m_linkedParticles[i]);

    }
    else
    {
      int r=planeSorting(normal,linkPosition[a],linkPosition[i]);
      if(r<=0)
      {
        keepList.push_back(m_linkedParticles[i]);
      }
      else
      {
        relinkList.push_back(m_linkedParticles[i]);
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

//  if(relinkList.size()<3)
//  {
//    std::cout<<"Not enough particles in relink list"<<std::endl;
//    std::cout<<"random particles "<<a<<' '<<b<<std::endl;
//    std::cout<<"b= "<<m_linkedParticles.size()<<std::endl;
//  }

  _particleList.push_back(std::unique_ptr<LinkedParticle> (new LinkedParticle(x,y,z,relinkList)));

  //get the new particles ID

  int newPartID=_particleList[_particleList.size()-1]->getID();

  //link all the particles to the new particle
  for(unsigned int i=0;i<relinkList.size();i++)
  {
    for(unsigned int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==relinkList[i])
      {
        _particleList[j]->link(newPartID);
        break;
      }
    }
  }

  //link both, parent and child, to each other
  m_linkedParticles=keepList;

  //delete links from to old particles

  for(unsigned int i=0;i<relinkList.size();i++)
  {
    //find correct linked particle in particle list
    for(unsigned int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==relinkList[i])
      {
        _particleList[j]->deleteLink(m_ID);
        break;
      }
    }
  }
  link(newPartID);

  m_foodLevel=0;
}


int LinkedParticle::getLinkCount()
{
  return m_linkedParticles.size();
}


void LinkedParticle::getPosFromLinks(std::vector<QVector3D> &_linkPos,std::vector<std::unique_ptr<LinkedParticle>> &_particleList)
{
  //looks for the Id in the particleList of the particle system and than gets the position
  //there should be a more efficient way of doing this
  _linkPos.clear();
  QVector3D tempVec;

  for(unsigned int i =0; i<m_linkedParticles.size();i++)
  {
    for(unsigned int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==m_linkedParticles[i])
      {
        _particleList[j]->getPos(tempVec);
        _linkPos.push_back(tempVec);
        break;

      }
    }
  }
}


// Initializes the counter (static member) to zero
int LinkedParticle::m_ID_counter(0);


// Gets the position of the particle in the list vector
int LinkedParticle::getPosInPS(std::vector<std::unique_ptr<LinkedParticle>> &_particleList)
{
  for (unsigned int i=0; i < _particleList.size(); i++)
  {
    if (_particleList[i]->getID() == m_ID)
    {
      return i;
    }
  }
}


bool LinkedParticle::collision(int _levels,QVector3D _testPosition,std::vector<std::unique_ptr<LinkedParticle>> &_particleList)
{
  int parent=getPosInPS(_particleList);
  //std::cout<<parent<<std::endl;
  std::vector<int> links;

  for(int j=0;j<=_levels;j++)
  {

    _particleList[parent]->getLinks(links);
    //for first particle
    if(links.size()==0){break;}
    if(parent==0){break;}
     //find parent in particle system
    for(int i=0;i<_particleList.size();i++)
    {

      if(_particleList[i]->getID()==links[0])
      {
        parent=i;
        break;
      }
    }
  }


  return _particleList[parent]->recursiveCollision(_testPosition,_particleList);



}




bool LinkedParticle::testCollision(QVector3D _particle,float _size)
{
  float distance=_particle.distanceToPoint(m_pos);
  if (distance<=_size/2)
  {
    //std::cout<<distance<<std::endl;
    return true;
  }
  else
  {
    return false;
  }

}

bool LinkedParticle::recursiveCollision(QVector3D _particle,std::vector<std::unique_ptr<LinkedParticle>> &_particleList)
{
  //tests for collision of the current particle
  if(testCollision(_particle,m_size))
  {
     return true;
  }

  //if the particle doesn't have any children it will return
  if(m_linkedParticles.size()<=1){return false;}

  //starting from 1 as first particle is mother particle
  for(int i=1;i<m_linkedParticles.size();i++)
  {
    //find particle in particle list
    for(int j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==m_linkedParticles[i])
      {
        //call the function recursivly
        if(_particleList[j]->recursiveCollision(_particle,_particleList))
        {
          return true;
        }
      }
    }
  }
  return false;


}


