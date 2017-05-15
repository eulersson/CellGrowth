// Qt
#include <QOpenGLFunctions_4_1_Core>

// Project
#include "SkyBox.h"

SkyBox::SkyBox(InputManager *_input_manager) : m_input_manager(_input_manager), m_blur_iterations(5)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::setBackground(QString _name)
{
  if (m_cubemap_texture->isCreated()) m_cubemap_texture->destroy();

  const QImage posx = QImage(QString(":/sky/%1_ft").arg(_name)).convertToFormat(QImage::Format_RGB888);
  const QImage posy = QImage(QString(":/sky/%1_up").arg(_name)).convertToFormat(QImage::Format_RGB888);
  const QImage posz = QImage(QString(":/sky/%1_rt").arg(_name)).convertToFormat(QImage::Format_RGB888);
  const QImage negx = QImage(QString(":/sky/%1_bk").arg(_name)).convertToFormat(QImage::Format_RGB888);
  const QImage negy = QImage(QString(":/sky/%1_dn").arg(_name)).convertToFormat(QImage::Format_RGB888);
  const QImage negz = QImage(QString(":/sky/%1_lf").arg(_name)).convertToFormat(QImage::Format_RGB888);


  if (posz.isNull()) qDebug("Null image");
  qDebug("%d %d %d",posz.width(), posz.height(), posz.depth());

  m_cubemap_texture->create();
  m_cubemap_texture->setSize(posx.width(), posx.height(), posx.depth());
  m_cubemap_texture->setFormat(QOpenGLTexture::RGB8_UNorm);
  m_cubemap_texture->allocateStorage();

  m_cubemap_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)posx.constBits(),0);
  m_cubemap_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)posy.constBits(),0);
  m_cubemap_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)posz.constBits(),0);
  m_cubemap_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)negx.constBits(),0);
  m_cubemap_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)negy.constBits(),0);
  m_cubemap_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)negz.constBits(),0);

  m_cubemap_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
  m_cubemap_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  m_cubemap_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

  m_cubemap_texture->generateMipMaps();

}

void SkyBox::setBlurIterations(uint _value)
{
  qDebug("changed blur to %d", _value);
  m_blur_iterations = _value;
}



void SkyBox::prepare(int _width, int _height, QOpenGLFunctions_4_1_Core* _funcs)
{
    //Creating a cube that goes around the scene.
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

  m_sky_program = new QOpenGLShaderProgram;
  m_sky_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/skybox.vert");
  m_sky_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/skybox.frag");
  m_sky_program->link();

  m_skybox_vao = new QOpenGLVertexArrayObject;
  m_skybox_vao->create();

  m_skybox_vbo.create();
  m_skybox_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_sky_program->bind();

  m_skybox_vao->bind();
  m_skybox_vbo.bind();

  m_skybox_vbo.allocate(points, 6*6*3 * sizeof(GLfloat));

  m_sky_program->enableAttributeArray("pos");

  m_sky_program->setAttributeBuffer("pos", GL_FLOAT, 0, 3);
  m_sky_program->setUniformValue("tSkyBox", 4);

  m_skybox_vao->release();

  m_sky_program->release();

  m_cubemap_texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
  setBackground("badomen");

  // ===========================================================================
  // Set offscreen rendering
  // ===========================================================================
  m_blur_program = new QOpenGLShaderProgram;
  m_blur_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/bgblur.vert");
  m_blur_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/bgblur.frag");
  m_blur_program->link();

  static const float quad[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
  };

  m_quad_vao = new QOpenGLVertexArrayObject;

  m_quad_vao->create();
  m_quad_vbo.create();
  m_quad_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

  m_quad_vao->bind();
  m_quad_vbo.bind();

  m_quad_vbo.allocate(quad, 30 * sizeof(GLfloat));

  m_blur_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
  m_blur_program->enableAttributeArray("position");

  m_blur_program->setAttributeBuffer("uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
  m_blur_program->enableAttributeArray("uv");

  m_quad_vao->release();

  m_painted_sky = new QOpenGLTexture(QOpenGLTexture::Target2D);
  m_painted_sky->setSize(_width, _height);
  m_painted_sky->setMinificationFilter(QOpenGLTexture::Linear);
  m_painted_sky->setMagnificationFilter(QOpenGLTexture::Linear);
  m_painted_sky->setFormat(QOpenGLTexture::RGB32F);
  m_painted_sky->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::Float32);

  m_fbo = new QOpenGLFramebufferObject(_width, _height);

  _funcs->glBindTexture(GL_TEXTURE_2D, m_painted_sky->textureId());
  _funcs->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_painted_sky->textureId(), 0);
  const GLenum blur_attachments[1] = {GL_COLOR_ATTACHMENT0};
  _funcs->glDrawBuffers(1, blur_attachments);

  // Finally check if framebuffer object is complete
  if (_funcs->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    qCritical("gBuffer FBO not complete!");

  m_fbo->bind();

  m_fbo->release();

  m_blur_program->setUniformValue("tInputBG", 0);
}

void SkyBox::draw(QOpenGLFunctions_4_1_Core* _funcs)
{
  m_fbo->bind();

  m_cubemap_texture->bind(4);
  m_painted_sky->bind(0);

    m_sky_program->bind();

    QMatrix4x4 V = m_input_manager->getViewMatrix();

    // Extract just the rotation part of the camera view matrix
    V = QMatrix4x4(V.row(0)[0], V.row(0)[1], V.row(0)[2], 0,
                   V.row(1)[0], V.row(1)[1], V.row(1)[2], 0,
                   V.row(2)[0], V.row(2)[1], V.row(2)[2], 0,
                   0          ,           0,           0, 1);

    m_sky_program->setUniformValue("Projection", m_input_manager->getProjectionMatrix());
    m_sky_program->setUniformValue("View", V);

    m_skybox_vao->bind();
      _funcs->glDrawArrays(GL_TRIANGLES, 0, 36);
    m_skybox_vao->release();

    m_sky_program->release();

   m_fbo->release();



  for (uint i = 1; i < m_blur_iterations; i++)
  {
    m_fbo->bind();
     m_painted_sky->bind(0);
     m_blur_program->bind();
     m_quad_vao->bind();
       _funcs->glDrawArrays(GL_TRIANGLES, 0, 6);
     m_quad_vao->release();
     m_blur_program->release();
    m_fbo->release();

  }



   _funcs->glDisable(GL_DEPTH_TEST);

   m_blur_program->bind();
   m_quad_vao->bind();
   m_painted_sky->bind(0);
   _funcs->glDrawArrays(GL_TRIANGLES, 0, 6);
   m_quad_vao->release();

   m_blur_program->release();

   _funcs->glEnable(GL_DEPTH_TEST);



}
