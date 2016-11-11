#include <iostream>

#include <QtGui/QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>

#include "particleswindow.h"

void push_indices(std::vector<GLushort>& indices, int sectors, int r, int s) {
    int curRow = r * sectors;
    int nextRow = (r+1) * sectors;
    int nextS = (s+1) % sectors;

    indices.push_back(curRow + s);
    indices.push_back(nextRow + s);
    indices.push_back(nextRow + nextS);

    indices.push_back(curRow + s);
    indices.push_back(nextRow + nextS);
    indices.push_back(curRow + nextS);
}

void createSpherePoints(std::vector<QVector3D>& vertices,
                  float radius, unsigned int rings, unsigned int sectors)
{
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(int r = 0; r < rings; ++r) {
        for(int s = 0; s < sectors; ++s) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            vertices.push_back(QVector3D(x,y,z) * radius);
        }
    }
}

void createSphere(std::vector<QVector3D>& vertices, std::vector<GLushort>& indices,
                  float radius, unsigned int rings, unsigned int sectors)
{
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(int r = 0; r < rings; ++r) {
        for(int s = 0; s < sectors; ++s) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            vertices.push_back(QVector3D(x,y,z) * radius);
            if(r < rings-1)
                push_indices(indices, sectors, r, s);
        }
    }
}

static const char *vertexShaderSource =
    "attribute highp vec3 posAttr;\n"
    "uniform mediump mat4 transformation;\n"
    "void main() {\n"
    "   gl_PointSize = 10.0;\n"
    "   gl_Position = transformation * vec4(posAttr, 1.0);\n"
    "}\n";

static const char *fragmentShaderSource =
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n";

ParticlesWindow::ParticlesWindow()
    : m_program(0)
    , m_frame(0)
{
    QSurfaceFormat format;
    format.setSamples(16);
    this->resize(720, 720);
    this->setFormat(format);
}

void ParticlesWindow::initialize()
{
    std::vector<GLushort> indices;
    std::vector<QVector3D> vertices;

    createSphere(vertices, indices, 0.01, 5, 5);

    m_sphere_vertices = new GLfloat[3*vertices.size()];
    m_sphere_indices = new GLuint[indices.size()];
    m_number_of_indices = indices.size();

    for (int i = 0; i < vertices.size(); i++)
    {
        m_sphere_vertices[3*i+0] = vertices[i].x();
        m_sphere_vertices[3*i+1] = vertices[i].y();
        m_sphere_vertices[3*i+2] = vertices[i].z();
    }
    for (int i = 0; i < indices.size(); i++)
    {
        m_sphere_indices[i] = indices[i];
    }


    // positions where to instance the spheres
    createSpherePoints(m_positions, 0.5, 100, 100);

    glViewport(0, 0, width(), height());

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_VAO = new QOpenGLVertexArrayObject(this);
    m_VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_VAO->create();
    m_VBO->create();
    m_EBO->create();

    m_VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    m_VAO->bind();
    m_VBO->bind();
    m_EBO->bind();

    m_VBO->allocate(m_sphere_vertices, vertices.size() * 3 * sizeof(GLfloat));
    m_EBO->allocate(m_sphere_indices, indices.size() * sizeof(GLuint));

    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
    m_program->enableAttributeArray("posAttr");

    m_VBO->release();
    m_VAO->release();
    m_EBO->release();
}

void ParticlesWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->bind();
    m_VAO->bind();

    for (unsigned int i = 0; i < m_positions.size(); i++)
    {
        QMatrix4x4 model;
        model.translate(m_positions[i].x(), m_positions[i].y(), m_positions[i].z());
        model.rotate((float)m_frame/2.0f, 0.0f, 1.0f, 0.0f);

        m_program->setUniformValue("transformation", model);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glDrawElements(GL_TRIANGLES, m_number_of_indices, GL_UNSIGNED_INT, 0);
    }

    m_VAO->release();
    m_program->release();

    ++m_frame;
}

void ParticlesWindow::keyPressEvent(QKeyEvent *ev)
{
    m_VBO->bind();
    m_positions.push_back(QVector3D(0.0f, -0.5f, 0.0f));
    //m_VBO->allocate(&m_vertices[0], 3 * m_particle_system.numParts * sizeof(GLfloat));
    m_VBO->release();
}
