#include "LinkedParticle.h"

// Default constructor puts it at origin
LinkedParticle::LinkedParticle():
  m_ID(m_ID_counter++)
{
  m_pos = QVector3D(0, 0, 0);

}

// Specify the location where we want to place it
LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z)
: m_ID(m_ID_counter++)
{
  m_pos = QVector3D(_x, _y, _z);
}


LinkedParticle::LinkedParticle(qreal _x,qreal _y,qreal _z,std::vector<int> _linkedParticles)
: m_ID(m_ID_counter++)
{
  m_pos = QVector3D(_x, _y, _z);
  m_linkedParticles=_linkedParticles;
}

// After force calculations are done, we advect the position
void LinkedParticle::advance()
{
  m_pos += m_vel;
}

// All the force calculation should happen in here
void LinkedParticle::calculate()
{
  //m_vel = 0.0005 * m_pos.normalized();
}

// For modifying the position of the particle
void LinkedParticle::setPos(qreal _x,qreal _y,qreal _z)
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

//adds linkedParticles ID to the linked Particle List
void LinkedParticle::link(int _ID)
{
  m_linkedParticles.push_back(_ID);
}

//Iterates through linked P. list and erases the input ID
void LinkedParticle::deleteLinke(int _ID)
{
  for(int i=0; i<m_linkedParticles.size();i++)
  {
    if(m_linkedParticles[i]==_ID)
      m_linkedParticles.erase(m_linkedParticles.begin()+i);
  }
}

//returns ID
int LinkedParticle::getID()
{
 return m_ID;
}

void LinkedParticle::getLinks(std::vector<int> &_returnList)
{
  _returnList=m_linkedParticles;
}

int LinkedParticle::planeSorting(QVector3D _normal, QVector3D _planePoint, QVector3D _testPoint)
{
  int d=_normal.x()*_planePoint.x()+_normal.y()*_planePoint.y()+_normal.z()*_planePoint.z();
  int r=_normal.x()*_testPoint.x()+_normal.y()*_testPoint.y()+_normal.z()*_testPoint.z()-d;
  return r;
}

void LinkedParticle::split(std::vector<LinkedParticle> &_particleList)

{

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int> distribution(0,m_linkedParticles.size());
  //holds all ID's of the partciles that are keept by the current particle
  std::vector<int> keepList;
  //holds all the ID's of the particles that are linked to the new particle
  std::vector<int> relinkList;
  // holds the positions of the linked particles
  std::vector<QVector3D> linkPosition;
  getPosFromLinks(linkPosition,_particleList);


  //pick two random particles out of the particle list
  //saving index number of it in list not Id or Pos
  int a=distribution(gen);
  int b=distribution(gen);

  QVector3D normal=QVector3D::normal(linkPosition[a],linkPosition[b]);

  //filling two arrays with links based on there position relative to the plane created by the two first particles
  for(int i=0;i<m_linkedParticles.size();i++)
  {
    if(i==a || i == b)
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
  m_linkedParticles=keepList;

  //delete links from to old particle







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

  for(int i =0; i<m_linkedParticles.size();i++)
  {
    for(int j=0;j<_particleList.size();j++)
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

int LinkedParticle::m_ID_counter(0);
