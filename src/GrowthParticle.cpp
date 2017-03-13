#include "GrowthParticle.h"

GrowthParticle::GrowthParticle():Particle()
{
  //qDebug("Growth Particle default constructor.");
}

GrowthParticle::GrowthParticle(qreal _x,
                               qreal _y,
                               qreal _z): Particle(_x,_y,_z)
{
  m_childrenTreshold=3;
  //qDebug("Growth Particle constructor passing in positions: %f,%f,%f", _x, _y, _z);
}

GrowthParticle::GrowthParticle(
    qreal _x,
    qreal _y,
    qreal _z,
    std::vector<unsigned int> _connectedParticles):Particle(_x,_y,_z,_connectedParticles)
{
  m_childrenTreshold=3;
  //qDebug("Growth Particle constructor passing in positions: %f,%f,%f and a list of"
         //"particles", _x, _y, _z);
}

void GrowthParticle::calculate(QVector3D _particleCentre, std::vector<std::unique_ptr<Particle> > &_particleList, QVector3D _averageDistance, std::vector<unsigned int> &_returnList)
{

}

void GrowthParticle::bulge(QVector3D _particleCentre)
{

}

void GrowthParticle::split(QVector3D _lightDirection, std::vector<std::unique_ptr<Particle> > &_particleList)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());

  //triggered by reaching the threshold
  //check length of linked list to see if the max particle treshold is reached
  if(m_connectedParticles.size() >= m_childrenTreshold)
  {return;}

  //create new particle

    //creating list for new particlen including mother particle
    std::vector<unsigned int> newLinkedParticles;
    //appends mother ID
    newLinkedParticles.push_back(m_ID);

    std::uniform_real_distribution<float> distributionX(m_pos[0]+0.001,_lightDirection[0]);
    std::uniform_real_distribution<float> distributionY(m_pos[1]+0.001,_lightDirection[1]);
    std::uniform_real_distribution<float> distributionZ(m_pos[2]+0.001,_lightDirection[2]);






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

    _particleList.push_back(std::unique_ptr<GrowthParticle> (new GrowthParticle(x,y,z,newLinkedParticles)));
    // add particle to links in original particle
    int new_ID = _particleList[_particleList.size()-1 ]->getID();
    m_connectedParticles.push_back(new_ID);

}

bool GrowthParticle::collision(int _levels,QVector3D _testPosition,std::vector<std::unique_ptr<Particle> > &_particleList)
{
  int parent=getPosInPS(_particleList);
  //std::cout<<parent<<std::endl;
  std::vector<unsigned int> links;

  for(int j=0;j<=_levels;j++)
  {

    _particleList[parent]->getConnectionsID(links);
    //for first particle
    if(links.size()==0){break;}
    if(parent==0){break;}
     //find parent in particle system
    for(size_t i = 0; i < _particleList.size(); i++)
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

bool GrowthParticle::testCollision(QVector3D _particle,float _size)
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

bool GrowthParticle::recursiveCollision(QVector3D _particle,std::vector<std::unique_ptr<Particle>> &_particleList)
{
  //tests for collision of the current particle
  if(testCollision(_particle,m_size))
  {
     return true;
  }

  //if the particle doesn't have any children it will return
  if(m_connectedParticles.size()<=1){return false;}

  //starting from 1 as first particle is mother particle
  for(size_t i=1;i<m_connectedParticles.size();i++)
  {
    //find particle in particle list
    for(size_t j=0;j<_particleList.size();j++)
    {
      if(_particleList[j]->getID()==m_connectedParticles[i])
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
