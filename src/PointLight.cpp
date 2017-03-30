#include "PointLight.h"

const int DIRECTION_X = 0;
const int DIRECTION_Y = 1;
const int DIRECTION_Z = 2;

const int ROTATION_Y = 3;
const int ROTATION_Z = 4;

const GLfloat SENSITIVITY = 0.04f;

PointLight::PointLight(QVector3D _position, QOpenGLShaderProgram *_manipshaderp, QOpenGLShaderProgram *_sunshaderp) :
    m_manip(_position, _manipshaderp)
{
    m_position=_position;
    m_sunshaderp=_sunshaderp;
    m_manipshaderp=_manipshaderp;
    // Set light representation position (single point)
    m_points[0]=0; m_points[1]=0; m_points[2]=0;
    updateModelMatrix();
}

int PointLight::compareUniqueColour(QVector3D _colour)
{
  return m_manip.compareUniqueColour(_colour);
}

void PointLight::createGeometry(QVector3D &_masterUniqueColour)
{
  // Setup light representation VBO VAO
  setupObject();
  // Setup manipulator geometry
  int amountOfColours=5;
  m_manip.createGeometry(getMultipleNewUniqueColour(amountOfColours, _masterUniqueColour), false);
}

void PointLight::draw()
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

void PointLight::drawBackBuffer()
{
  m_manipshaderp->bind();
  m_manipshaderp->setUniformValue("backRender", true);
  GLint modelLoc = m_manipshaderp->uniformLocation("model");
  m_manipshaderp->setUniformValue(modelLoc, m_model);
  m_manipshaderp->setUniformValue("backRender", false);
  m_manip.drawBackBuffer();
}

void PointLight::processMouseMovement(float _offsetx, float _offsety, float _offsetz, QVector3D _campos, QMatrix4x4 _view)
{

  QVector3D movement;
  switch(m_manip.getClickedAxis())
  {
    case DIRECTION_X:
      {
        if(_campos.x()<m_position.z()){
            _offsety=-_offsety;
        }
        movement = _offsetx*SENSITIVITY*QVector3D(1,0,0);
        break;
      }


    case DIRECTION_Y:
      {
        movement = _offsety*SENSITIVITY*QVector3D(0,1,0);
        break;
      }

    case DIRECTION_Z:
      {

        if(_campos.x()<m_position.z()){
            _offsety=-_offsety;
        }

        movement = _offsetz*SENSITIVITY*QVector3D(0,0,1);
        break;
      }
  }
  m_position+=movement;
  updateModelMatrix();
}

void PointLight::setClicked(QVector3D uColour, bool _state)
{
  m_manip.setClicked(uColour, _state);
}

void PointLight::setHover(int id)
{
  m_manip.setHover(id);
}

QVector3D PointLight::getPosition()
{
  return m_position;
}

Manipulator PointLight::getManipulator()
{
  return m_manip;
}

void PointLight::getMainProgram(QOpenGLShaderProgram **retshader)
{
  *retshader=m_manipshaderp;
}

void PointLight::setupObject()
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

void PointLight::updateModelMatrix()
{
  m_model(0,3)=m_position.x();
  m_model(1,3)=m_position.y();
  m_model(2,3)=m_position.z();
}
