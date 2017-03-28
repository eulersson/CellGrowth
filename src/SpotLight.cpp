#include "SpotLight.h"

const int DIRECTION_X = 0;
const int DIRECTION_Y = 1;
const int DIRECTION_Z = 2;

const int ROTATION_Y = 3;
const int ROTATION_Z = 4;

// Manipulator move sensitivity
const GLfloat SENSITIVITY = 0.04f;

SpotLight::SpotLight(QVector3D _position, QOpenGLShaderProgram *_manipshaderp, QOpenGLShaderProgram *_sunshaderp) :
    m_manip(_position, _manipshaderp),
    m_model()
{
  m_position=_position;
  m_sunshaderp=_sunshaderp;
  m_manipshaderp=_manipshaderp;
  // Set light representation position (single point)
//    m_points[0]=_position.x(); m_points[1]=_position.y(); m_points[2]=_position.z();
  m_points[0]=0; m_points[1]=0; m_points[2]=0;
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

void SpotLight::createGeometry(QOpenGLContext *_context, QVector3D &_masterUniqueColour)
{
  // Setup light representation VBO VAO
  setupObject(_context);
  // Setup manipulator geometry
  int amountOfColours=5;
  m_manip.createGeometry(_context, getMultipleNewUniqueColour(amountOfColours, _masterUniqueColour));
}

void SpotLight::draw()
{
  // LIGHT
  m_vao->bind();
  m_sunshaderp->bind();
  GLint modelLoc = m_sunshaderp->uniformLocation("model");
  m_sunshaderp->setUniformValue(modelLoc, m_model);
  // Setup/Draw
  glEnable(GL_POINT_SPRITE);
  glPointSize(22.0f);
  glDrawArrays(GL_POINTS, 0, 1);
  // Release and reset everything
  m_sunshaderp->release();
  m_vao->release();
  glDisable(GL_POINT_SPRITE);
  glPointSize(0.0f);

  // MANIPULATOR
  m_manipshaderp->bind();
  modelLoc = m_manipshaderp->uniformLocation("model");
  m_manipshaderp->setUniformValue(modelLoc, m_model);
  // Draw manipulator
  m_manip.draw();
}

void SpotLight::drawBackBuffer()
{
  m_manipshaderp->bind();
  GLint modelLoc = m_manipshaderp->uniformLocation("model");
  m_manipshaderp->setUniformValue(modelLoc, m_model);
  m_manip.drawBackBuffer();
}

void SpotLight::processMouseMovement(float _offsetx, float _offsety, float _offsetz, QVector3D _campos, QMatrix4x4 _view)
{
//  updateModelMatrix();
//  m_position+=m_manip.processMouseMovement(_offsetx, _offsety, _offsetz,
//                                           m_x, m_y, m_z);

  QVector3D camRight=QVector3D(_view(0,0), _view(0,1), _view(0,2));
  QVector3D camUp=QVector3D(_view(1,0), _view(1,1), _view(1,2));
  QVector3D camFront=QVector3D(_view(2,0), _view(2,1), _view(2,2));

  float offset;

  QVector3D movement;
  switch(m_manip.getClickedAxis())
  {
    case DIRECTION_X:
      {
        if(_campos.x()<m_position.z()){
            _offsety=-_offsety;
        }

//        float dotprod=QVector3D::dotProduct(camRight, m_x);
//        offset = _offsetx*dotprod + (1-dotprod)*_offsety;

        movement = _offsetx*SENSITIVITY*m_x;
        break;
      }


    case DIRECTION_Y:
      {



        float dotprod;
        float dotprodz=QVector3D::dotProduct(camRight, m_z);
        float dotprodx=QVector3D::dotProduct(camRight, m_x);
        if(dotprodx>dotprodz){dotprod=dotprodx;} else {dotprod=dotprodz;}
        if(dotprod>.5)
          {
            offset=_offsety*dotprod;
          }
        else{

            offset=-_offsety;
          }

        movement = offset*SENSITIVITY*m_y;
        break;
      }


    case DIRECTION_Z:
      {

        if(_campos.x()<m_position.z()){
            _offsety=-_offsety;
        }
        float dotprod=QVector3D::dotProduct(camRight, m_z);
        offset = _offsetz*dotprod + (1-dotprod)*_offsety;

        movement = offset*SENSITIVITY*m_z;
        break;
      }
  }
  m_position+=movement;
  updateModelMatrix();

  rotate(_offsetx, _offsety, _offsetz);
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

void SpotLight::rotate(float _offsetx, float _offsety, float _offsetz)
{

  float rotSensitivity=0.5f;

  switch (m_manip.getClickedAxis())
  {
    case ROTATION_Y:
      {

      QQuaternion rotq=create_from_angle(0,1,0,qDegreesToRadians(_offsetx*rotSensitivity));

      quat=quat*rotq;

//      // Normalize
//      QQuaternion tmpquat=QQuaternion();
//      float qw=quat.scalar(); float qx=quat.x(); float qy=quat.y(); float qz=quat.z();
//      const float n = 1.0f/sqrt(qx*qx+qy*qy+qz*qz+qw*qw);
//      tmpquat.setScalar (qw*n);
//      tmpquat.setX      (qx*n);
//      tmpquat.setY      (qy*n);
//      tmpquat.setZ      (qz*n);

      break;
      }

    case ROTATION_Z:
      {
      QQuaternion rotq=create_from_angle(0,0,1,qDegreesToRadians(_offsety*rotSensitivity));

      quat=quat*rotq;
      break;
      }

  }


  QMatrix3x3 rotmat=quat.toRotationMatrix();
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
//  // Vector must be reflected along the main x axis to be correct.
  //m_x=m_x-2*(m_x*QVector3D(1,0,0))*QVector3D(1,0,0);

  m_y.setX(rotmat(1,0));
  m_y.setY(rotmat(1,1));
  m_y.setZ(rotmat(1,2));
  // Vector must be reflected along the main y axis to be correct.
  //m_y=m_y-2*(m_y*QVector3D(0,1,0))*QVector3D(0,1,0);

  m_z.setX(rotmat(2,0));
  m_z.setY(rotmat(2,1));
  m_z.setZ(rotmat(2,2));
  // Vector must be reflected along the main x axis to be correct.
  //m_z=m_z-2*(m_z*QVector3D(0,0,1))*QVector3D(0,0,1);

  qDebug("%f, %f, %f", m_z.x(), m_z.y(), m_z.z());







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

void SpotLight::setupObject(QOpenGLContext *_context)
{
  // VAO / VBO
  m_vao = new QOpenGLVertexArrayObject();
  m_vao->create();
  m_vao->bind();
  m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  m_vbo->create();
  m_vbo->bind();
  m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
  m_vbo->allocate(&m_points[0], 3* sizeof(GLfloat)); // Allocate enogh place for all data
  // Set shader attributes
  m_sunshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
  m_sunshaderp->enableAttributeArray("posAttr");

  m_vao->release();
}

void SpotLight::updateModelMatrix()
{
  m_model(0,3)=m_position.x();
  m_model(1,3)=m_position.y();
  m_model(2,3)=m_position.z();

}
