#include "SpotLight.h"

const int DIRECTION_X = 0;
const int DIRECTION_Y = 1;
const int DIRECTION_Z = 2;

const int ROTATION_Y = 3;
const int ROTATION_Z = 4;

// Manipulator move sensitivity.
const GLfloat SENSITIVITY = 0.04f;
// Must be between 1 and 179.
const GLfloat CONE_ANGLE = 80.0f;

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

void SpotLight::createGeometry(QVector3D &_masterUniqueColour)
{
  // Setup light representation VBO VAO
  setupObject();
  // Setup manipulator geometry
  int amountOfColours=5;
  m_manip.createGeometry(getMultipleNewUniqueColour(amountOfColours, _masterUniqueColour), true);
}

void SpotLight::draw()
{
  // LIGHT
  m_vao->bind();
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("model", m_model);
  m_manipshaderp->setUniformValue("flatRender", true);
  m_manipshaderp->setUniformValue("backRender", false);
  // Setup/Draw
  glDrawArrays(GL_TRIANGLES, 0, m_numberOfPoints);
  // Release and reset everything
  m_manipshaderp->release();
  m_vao->release();

  // MANIPULATOR
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("model", m_model);
  m_manipshaderp->setUniformValue("flatRender", false);
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

void SpotLight::processMouseMovement(float _offsetx, float _offsety, float _offsetz, QVector3D _campos, QMatrix4x4 _view)
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

        movement = offset*SENSITIVITY*m_x;
        break;
      }


    case DIRECTION_Y:
      {
        float dotprodx = QVector3D::dotProduct(m_y, camRight);
        float dotprody = QVector3D::dotProduct(m_y, camUp);
        float dotprodz = QVector3D::dotProduct(m_y, camFront);
        float offset=dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz;

        movement = offset*SENSITIVITY*m_y;
        break;
      }


    case DIRECTION_Z:
      {

        float dotprodx = QVector3D::dotProduct(m_z, camRight);
        float dotprody = QVector3D::dotProduct(m_z, camUp);
        float dotprodz = QVector3D::dotProduct(m_z, camFront);
        float offset=dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz;

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

      m_quat=m_quat*rotq;

      break;
      }

    case ROTATION_Z:
      {
      QQuaternion rotq=create_from_angle(0,0,1,qDegreesToRadians(_offsety*rotSensitivity));

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






void SpotLight::setupObject()
{

  float coneangle=90-CONE_ANGLE/2.0f;

  // Create geomtry
  std::vector<QVector3D> vertices;
  unsigned int segments = 20;
  float amt=(2*M_PI)/segments;
  float r = CONE_ANGLE/90.0f;
  float thickness=0.04f; thickness++;
  float length = 3.0f*sin(qDegreesToRadians(coneangle));
  QVector3D topPoint = QVector3D(m_points[0], m_points[1], m_points[2]);
//  for(unsigned int i = 0; i < sectors; i++) {

//    float angle = (2 * M_PI) * ((float)i / (float)sectors);
//    float s = radius * sin(angle);
//    float c = radius * cos(angle);

//    float angleNext = (2 * M_PI) * ((float)(i + 1) / (float)sectors);
//    float s2 = radius * sin(angleNext);
//    float c2 = radius * cos(angleNext);

//    float x = length;
//    float y = s;
//    float z = c;

//    float x2 = length;
//    float y2 = s2;
//    float z2 = c2;

//    vertices.push_back(QVector3D(x, y, z));
//    vertices.push_back(QVector3D(x2, y2, z2));
//    vertices.push_back(topPoint);
//  }

  QVector3D op(length,0,0);

  QVector3D lastPoint = QVector3D(op.x(),op.y(),op.z());
  for(size_t i=0; i<=segments;i++)
  {
    float angle = amt*i;

    float x=0;
    float z=r*cos(angle);
    float y=r*sin(angle);
    float lx=1;
    float lz=thickness;
    float ly=thickness;

    if(lastPoint==QVector3D(op.x(),op.y(),op.z()))
    {
      lastPoint=QVector3D(x,y,z);
      continue;
    }


    vertices.push_back(QVector3D(x,y,z)+QVector3D(op.x(),op.y(),op.z()));
    vertices.push_back(lastPoint+QVector3D(op.x(),op.y(),op.z()));
    vertices.push_back(QVector3D(lx, ly, lz)*lastPoint+QVector3D(op.x(),op.y(),op.z()));

    vertices.push_back(QVector3D(x,y,z)+QVector3D(op.x(),op.y(),op.z()));
    vertices.push_back(QVector3D(lx, ly, lz)*lastPoint+QVector3D(op.x(),op.y(),op.z()));
    vertices.push_back(QVector3D(x*lx, y*ly, z*lz)+QVector3D(op.x(),op.y(),op.z()));

    lastPoint=QVector3D(x,y,z);


  }

  // Make geometry OpenGL readable
  std::vector<GLfloat> m_pointPosArray;
  m_numberOfPoints = vertices.size();
  for (size_t i = 0; i < m_numberOfPoints; i++)
  {
    m_pointPosArray.push_back(vertices[i].x());
    m_pointPosArray.push_back(vertices[i].y());
    m_pointPosArray.push_back(vertices[i].z());
  }



  // VAO / VBO
  m_vao = new QOpenGLVertexArrayObject();
  m_vao->create();
  m_vao->bind();
  m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  m_vbo->create();
  m_vbo->bind();
  m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
  m_vbo->allocate(&m_pointPosArray[0], 3*m_numberOfPoints* sizeof(GLfloat)); // Allocate enogh place for all data
  // Set shader attributes
  m_manipshaderp->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
  m_manipshaderp->enableAttributeArray("posAttr");
  m_vao->release();
}

void SpotLight::updateModelMatrix()
{
  m_model(0,3)=m_position.x();
  m_model(1,3)=m_position.y();
  m_model(2,3)=m_position.z();

}
