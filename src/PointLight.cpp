#include "PointLight.h"

PointLight::PointLight(QVector3D _position, QOpenGLShaderProgram *_manipshaderp, QOpenGLShaderProgram *_sunshaderp) :
    m_manip(_position, _manipshaderp)
{
    m_position=_position;
    m_sunshaderp=_sunshaderp;
    m_manipshaderp=_manipshaderp;
    // Set light representation position (single point)
    m_points[0]=_position.x(); m_points[1]=_position.y(); m_points[2]=_position.z();
    updateModelMatrix(0,0,0);
}

int PointLight::compareUniqueColour(QVector3D _colour)
{
  return m_manip.compareUniqueColour(_colour);
}

void PointLight::createGeometry(QOpenGLContext *_context, QVector3D &_masterUniqueColour)
{
  // Setup light representation VBO VAO
  setupObject(_context);
  // Setup manipulator geometry
  int amountOfColours=5;
  m_manip.createGeometry(_context, getMultipleNewUniqueColour(amountOfColours, _masterUniqueColour));
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
  GLint modelLoc = m_manipshaderp->uniformLocation("model");
  m_manipshaderp->setUniformValue(modelLoc, m_model);
  m_manip.drawBackBuffer();
}

void PointLight::processMouseMovement(float offsetx, float _offsety, float _offsetz, QVector3D _campos, QMatrix4x4 _view)
{
  updateModelMatrix(offsetx, _offsety, _offsetz);
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

void PointLight::setupObject(QOpenGLContext *_context)
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

void PointLight::updateModelMatrix(float offsetx, float _offsety, float _offsetz)
{
  QVector3D pointPos=m_position;
  m_model.setToIdentity();
  m_model.translate(pointPos);
  GLfloat angle = 0.0f;
  m_model.rotate(angle, QVector3D(1,0.3,0.5));
  m_position+=m_manip.processMouseMovement(offsetx, _offsety, _offsetz,
                                          QVector3D(1,0,0), QVector3D(0,1,0), QVector3D(0,0,1));
}
