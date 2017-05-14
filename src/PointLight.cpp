#include "PointLight.h"

PointLight::PointLight(
    QVector3D _position,
    QOpenGLShaderProgram *_manipshaderp,
    QOpenGLShaderProgram *_sunshaderp) :
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
  // Passing -1 to indicate the light is non-rotatable.
  m_manip.createGeometry(getMultipleNewUniqueColour(amountOfColours, _masterUniqueColour),
                         -1);
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
  glPointSize(8.0f);
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
  m_manipshaderp->setUniformValue("lightScale", lightScale);
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


void PointLight::processMouseMovement(float _offsetx,
                                      float _offsety,
                                      float _offsetz,
                                      QMatrix4x4 _view,
                                      QMatrix4x4 _proj)
{

  GLfloat scaleModifier = 0.03;
  GLfloat w=(_proj * _view * m_model * QVector4D(0,0,0,1)).w();
  w *= scaleModifier;

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
        QVector3D right = QVector3D(1,0,0);
        float dotprodx = QVector3D::dotProduct(right, camRight);
        float dotprody = QVector3D::dotProduct(right, camUp);
        float dotprodz = QVector3D::dotProduct(right, camFront);
        float offset=(dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz)*w;

        movement = offset*m_sensitivity*right;
        break;
      }


    case DIRECTION_Y:
      {
        QVector3D up = QVector3D(0,1,0);
        float dotprodx = QVector3D::dotProduct(up, camRight);
        float dotprody = QVector3D::dotProduct(up, camUp);
        float dotprodz = QVector3D::dotProduct(up, camFront);
        float offset=(dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz)*w;

        movement = offset*m_sensitivity*up;
        break;
      }

    case DIRECTION_Z:
      {

        QVector3D front = QVector3D(0,0,1);
        float dotprodx = QVector3D::dotProduct(front, camRight);
        float dotprody = QVector3D::dotProduct(front, camUp);
        float dotprodz = QVector3D::dotProduct(front, camFront);
        float offset=(dotprodx*_offsetx + dotprody*_offsety + dotprodz*_offsetz)*w;

        movement = offset*m_sensitivity*front;
        break;
      }
  }
  m_position+=movement;
  updateModelMatrix();
}


void PointLight::setClicked(QVector3D _uColour, bool _state)
{
  m_manip.setClicked(_uColour, _state);
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
