#include "SkyBox.h"

SkyBox::SkyBox(InputManager *_input_manager) : m_input_manager(_input_manager)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::prepare()
{
  GLfloat points[] = {
    -1.0f ,  1.0f , -1.0f ,
    -1.0f , -1.0f , -1.0f ,
     1.0f , -1.0f , -1.0f ,
     1.0f , -1.0f , -1.0f ,
     1.0f ,  1.0f , -1.0f ,
    -1.0f ,  1.0f , -1.0f ,

    -1.0f , -1.0f ,  1.0f ,
    -1.0f , -1.0f , -1.0f ,
    -1.0f ,  1.0f , -1.0f ,
    -1.0f ,  1.0f , -1.0f ,
    -1.0f ,  1.0f ,  1.0f ,
    -1.0f , -1.0f ,  1.0f ,

     1.0f , -1.0f , -1.0f ,
     1.0f , -1.0f ,  1.0f ,
     1.0f ,  1.0f ,  1.0f ,
     1.0f ,  1.0f ,  1.0f ,
     1.0f ,  1.0f , -1.0f ,
     1.0f , -1.0f , -1.0f ,

    -1.0f , -1.0f ,  1.0f ,
    -1.0f ,  1.0f ,  1.0f ,
     1.0f ,  1.0f ,  1.0f ,
     1.0f ,  1.0f ,  1.0f ,
     1.0f , -1.0f ,  1.0f ,
    -1.0f , -1.0f ,  1.0f ,

    -1.0f ,  1.0f , -1.0f ,
     1.0f ,  1.0f , -1.0f ,
     1.0f ,  1.0f ,  1.0f ,
     1.0f ,  1.0f ,  1.0f ,
    -1.0f ,  1.0f ,  1.0f ,
    -1.0f ,  1.0f , -1.0f ,

    -1.0f , -1.0f , -1.0f ,
    -1.0f , -1.0f ,  1.0f ,
     1.0f , -1.0f , -1.0f ,
     1.0f , -1.0f , -1.0f ,
    -1.0f , -1.0f ,  1.0f ,
     1.0f , -1.0f ,  1.0f
  };

  m_program = new QOpenGLShaderProgram;
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "resources/shaders/skybox.vert");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "resources/shaders/skybox.frag");
  m_program->link();

  m_vao = new QOpenGLVertexArrayObject;
  m_vao->create();

  m_vbo.create();
  m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_program->bind();

  m_vao->bind();
  m_vbo.bind();

  m_vbo.allocate(points, 6*6*3 * sizeof(GLfloat));

  m_program->enableAttributeArray("pos");

  m_program->setAttributeBuffer("pos", GL_FLOAT, 0, 3);
  m_program->setUniformValue("tSkyBox", 8);

  m_vao->release();

  m_program->release();

  const QImage posx = QImage("resources/cubemaps/forest/posx.jpg").convertToFormat(QImage::Format_RGB888);
  const QImage posy = QImage("resources/cubemaps/forest/posy.jpg").convertToFormat(QImage::Format_RGB888);
  const QImage posz = QImage("resources/cubemaps/forest/posz.jpg").convertToFormat(QImage::Format_RGB888);
  const QImage negx = QImage("resources/cubemaps/forest/negx.jpg").convertToFormat(QImage::Format_RGB888);
  const QImage negy = QImage("resources/cubemaps/forest/negy.jpg").convertToFormat(QImage::Format_RGB888);
  const QImage negz = QImage("resources/cubemaps/forest/negz.jpg").convertToFormat(QImage::Format_RGB888);

  m_texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
  if (posz.isNull()) qDebug("Null image");
  qDebug("%d %d %d",posz.width(), posz.height(), posz.depth());

  m_texture->create();
  m_texture->setSize(posx.width(), posx.height(), posx.depth());
  m_texture->setFormat(QOpenGLTexture::RGB8_UNorm);
  m_texture->allocateStorage();

  m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)posx.constBits(),0);
  m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)posy.constBits(),0);
  m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)posz.constBits(),0);
  m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)negx.constBits(),0);
  m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)negy.constBits(),0);
  m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)negz.constBits(),0);

  m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
  m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  m_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

  m_texture->generateMipMaps();
}

void SkyBox::draw()
{
  glActiveTexture(GL_TEXTURE8);
  m_texture->bind();

    m_program->bind();

    QMatrix4x4 V = m_input_manager->getViewMatrix();

    // Extract just the rotation part of the camera view matrix
    V = QMatrix4x4(V.row(0)[0], V.row(0)[1], V.row(0)[2], 0,
                   V.row(1)[0], V.row(1)[1], V.row(1)[2], 0,
                   V.row(2)[0], V.row(2)[1], V.row(2)[2], 0,
                   0          ,           0,           0, 1);

    m_program->setUniformValue("Projection", m_input_manager->getProjectionMatrix());
    m_program->setUniformValue("View", V);

    m_vao->bind();
      glDrawArrays(GL_TRIANGLES, 0, 36);
    m_vao->release();

    m_program->release();
}
