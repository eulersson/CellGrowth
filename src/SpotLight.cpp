#include "SpotLight.h"


SpotLight::SpotLight(
    QVector3D _position,
    QOpenGLShaderProgram *_manipshaderp,
    QOpenGLShaderProgram *_sunshaderp) :
    m_coneangle(80.0f),
    m_manip(_position, _manipshaderp),
    m_model()
{
  m_position=_position;
  m_sunshaderp=_sunshaderp;
  m_manipshaderp=_manipshaderp;
  m_worldUp=QVector3D(0,1,0);

  m_x=QVector3D(1,0,0);
  m_y=QVector3D(0,1,0);
  m_z=QVector3D(0,0,1);


  updateModelMatrix();
}

int SpotLight::compareUniqueColour(QVector3D _colour)
{
  return m_manip.compareUniqueColour(_colour);
}

void SpotLight::createGeometry(QVector3D &_masterUniqueColour)
{
  // Setup manipulator geometry
  int amountOfColours=5;
  m_manip.createGeometry(getMultipleNewUniqueColour(amountOfColours,_masterUniqueColour),
                            m_coneangle);
}


void SpotLight::draw()
{
  // MANIPULATOR
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("model", m_model);
  // Draw manipulator
  m_manip.draw();
  m_manipshaderp->release();
}

void SpotLight::drawBackBuffer()
{

  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("backRender", true);
  GLint modelLoc = m_manipshaderp->uniformLocation("model");
  m_manipshaderp->setUniformValue(modelLoc, m_model);
  m_manip.drawBackBuffer();
  m_manipshaderp->setUniformValue("backRender", false);
  m_manipshaderp->release();
}


void SpotLight::processMouseMovement(float _offsetx,
                                     float _offsety,
                                     float _offsetz,
                                     QMatrix4x4 _view)
{

  QVector3D camRight=QVector3D(_view(0,0), _view(0,1), _view(0,2));
  QVector3D camUp=QVector3D(_view(1,0), _view(1,1), _view(1,2));
  QVector3D camFront=QVector3D(_view(2,0), _view(2,1), _view(2,2));
  QVector3D movement;
  switch(m_manip.getClickedAxis())
  {
    case DIRECTION_X:
      {
        // Caluculate how much of each offset should be used to move the light.
        // Dot product will be negative if vectors are moving towards each other,
        // so flipping the vectors is not necessary.
        float dotprodx = QVector3D::dotProduct(m_x, camRight);
        float dotprody = QVector3D::dotProduct(m_x, camUp);
        float dotprodz = QVector3D::dotProduct(m_x, camFront);
        float offset=dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz;

        movement = offset*m_sensitivity*m_x;
        break;
      }


    case DIRECTION_Y:
      {
        float dotprodx = QVector3D::dotProduct(m_y, camRight);
        float dotprody = QVector3D::dotProduct(m_y, camUp);
        float dotprodz = QVector3D::dotProduct(m_y, camFront);
        float offset=dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz;

        movement = offset*m_sensitivity*m_y;
        break;
      }


    case DIRECTION_Z:
      {

        float dotprodx = QVector3D::dotProduct(m_z, camRight);
        float dotprody = QVector3D::dotProduct(m_z, camUp);
        float dotprodz = QVector3D::dotProduct(m_z, camFront);
        float offset=dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz;

        movement = offset*m_sensitivity*m_z;
        break;
      }
  }
  m_position+=movement;
  updateModelMatrix();

  rotate(_offsetx, _offsety);
}

QQuaternion SpotLight::create_from_angle(const double &xx, const double &yy, const double &zz, const double &a)
{
    // Here we calculate the sin( theta / 2) once for optimization
    double factor = sin( a / 2.0 );

    // Calculate the x, y and z of the quaternion
    double x = xx * factor;
    double y = yy * factor;
    double z = zz * factor;

    // Calcualte the w value by cos( theta / 2 )
    double w = cos( a / 2.0 );

    QQuaternion ret = QQuaternion(w, x, y, z);
    ret.normalize();
    return ret;
}

void SpotLight::rotate(float _offsetx, float _offsety)
{

  float rotm_sensitivity=0.5f;

  switch (m_manip.getClickedAxis())
  {
    case ROTATION_Y:
      {

      QQuaternion rotq=create_from_angle(0,1,0,qDegreesToRadians(_offsetx*rotm_sensitivity));

      m_quat=m_quat*rotq;

      break;
      }

    case ROTATION_Z:
      {
      QQuaternion rotq=create_from_angle(0,0,1,qDegreesToRadians(_offsety*rotm_sensitivity));

      m_quat=m_quat*rotq;
      break;
      }

  }


  QMatrix3x3 rotmat=m_quat.toRotationMatrix();
  m_model(0,0)=rotmat(0,0);
  m_model(0,1)=rotmat(0,1);
  m_model(0,2)=rotmat(0,2);

  m_model(1,0)=rotmat(1,0);
  m_model(1,1)=rotmat(1,1);
  m_model(1,2)=rotmat(1,2);

  m_model(2,0)=rotmat(2,0);
  m_model(2,1)=rotmat(2,1);
  m_model(2,2)=rotmat(2,2);

  rotmat=rotmat.transposed();

  m_x.setX(rotmat(0,0));
  m_x.setY(rotmat(0,1));
  m_x.setZ(rotmat(0,2));

  m_y.setX(rotmat(1,0));
  m_y.setY(rotmat(1,1));
  m_y.setZ(rotmat(1,2));

  m_z.setX(rotmat(2,0));
  m_z.setY(rotmat(2,1));
  m_z.setZ(rotmat(2,2));


}

void SpotLight::setClicked(QVector3D uColour, bool _state)
{
  m_manip.setClicked(uColour, _state);

  if(_state==false)
  {


  }
}

void SpotLight::setHover(int id)
{
  m_manip.setHover(id);
}

QVector3D SpotLight::getPosition()
{
  return m_position;
}

Manipulator SpotLight::getManipulator()
{
  return m_manip;
}

void SpotLight::getMainProgram(QOpenGLShaderProgram **retshader)
{
  *retshader=m_manipshaderp;
}



void SpotLight::updateModelMatrix()
{
  m_model(0,3)=m_position.x();
  m_model(1,3)=m_position.y();
  m_model(2,3)=m_position.z();

}
