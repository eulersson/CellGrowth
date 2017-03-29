

////////////////////////////////////////////////////////////////////////////////
/// @file GLWindow.h
/// @author Ramon Blanquer
/// @author Fanny Marstrom
/// @author Carola Gille
/// @version 0.0.2
/// @class GLWindow
/// @brief Handles all the windowing related functionality and connects to the UI
////////////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_H
#define WINDOW_H

// Standard
#include <array>
#include <random>

// Qt
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QWheelEvent>

#include <QMainWindow>

// Project
#include "ParticleSystem.h"
#include "InputManager.h"

////////////////////////////////////////////////////////////////////////////////
/// @class Window
/// @brief Handles all the windowing related functionality.
///
/// Subclasses QOpenGLWindow which wraps all the functionality that allows
/// QWindow to render OpenGL graphics.
////////////////////////////////////////////////////////////////////////////////



class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core

{
  Q_OBJECT

public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Constructor
  /// @param[in] parent Sets the upper class parent
  //////////////////////////////////////////////////////////////////////////////
  //GLWindow(QMainWindow *_parent);
  GLWindow(QWidget *_parent);


  //////////////////////////////////////////////////////////////////////////////
  /// @brief Destructor
  //////////////////////////////////////////////////////////////////////////////
  ~GLWindow();






private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Internally calls the Scene::initialize() of the bound Scene.
  //////////////////////////////////////////////////////////////////////////////
  void initializeGL();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Internally calls the Scene::paint() of the bound Scene.
  //////////////////////////////////////////////////////////////////////////////
  void paintGL();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Updates the Scene's Viewport accordingly to the new Window size.
  //////////////////////////////////////////////////////////////////////////////
  void resizeGL(int _w, int _h);


  //////////////////////////////////////////////////////////////////////////////
  /// @brief Particle system associated with the scene.
  //////////////////////////////////////////////////////////////////////////////
  ParticleSystem m_ps;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Manager processing inputs and handling the camera.
  //////////////////////////////////////////////////////////////////////////////
  InputManager* m_input_manager;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief List of all scene objects. Not including particles.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<std::shared_ptr<SelectObject>> m_object_list;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief vec3 to store light position.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_lightPos;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief View position to store camera position.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_viewPos;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Model matrix for the particle system.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_model_matrix;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief View matrix for the particle system.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_view_matrix;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Projection matrix for the particle system.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_projection_matrix;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Model-View matrix
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_mv;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Model-View-Project matrix
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_mvp;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Framebuffer object we write to for the deferred shading.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLFramebufferObject *m_fbo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Quad shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_quad_program;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Particles shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_part_program;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Particle links shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_links_program;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Manipulator shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_manipulator_program;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light representation shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_sun_program;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief VAO that will store the state for drawing the quad.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLVertexArrayObject *m_quad_vao;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief VAO that will store the state for drawing the particles.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLVertexArrayObject *m_part_vao;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief VAO that will store state for drawing particle links.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLVertexArrayObject *m_links_vao;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief VBO buffer that stores the point data for the quad.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLBuffer m_quad_vbo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief VBO buffer that stores the point data for the particles.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLBuffer m_part_vbo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief EBO that stores the indices of the particles to draw links for.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLBuffer m_links_ebo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Buffer object holding the the vertex data of the sphere on GPU.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLBuffer m_sphere_vbo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Index for the RenderPass subroutine to be called.
  //////////////////////////////////////////////////////////////////////////////
  GLuint m_activeRenderPassIndex;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Index for RenderPass subroutine that draws world-space positions.
  //////////////////////////////////////////////////////////////////////////////
  GLuint m_AOIndex;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Index for RenderPass subroutine that draws depth.
  //////////////////////////////////////////////////////////////////////////////
  GLuint m_xRayIndex;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Index for RenderPass subroutine that draws diffuse.
  //////////////////////////////////////////////////////////////////////////////
  GLuint m_ADSIndex;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Renders just the normals.
  //////////////////////////////////////////////////////////////////////////////
  GLuint m_normalIndex;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief All the particle positions and radius ready for OpenGL to be drawn
  /// as instance position of the geometry defined in m_sphere_data.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<GLfloat> m_particle_data;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Hold the indices of the particles for which we will be drawing
  /// lines between them. In other words, this draws the links where one exists.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<uint> m_links_data;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Vertex data of the shape that will define an individual particle.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<GLfloat> m_sphere_data;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Toggled with key L. If true will render the links between
  /// particles.
  //////////////////////////////////////////////////////////////////////////////
  bool m_draw_links;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Array of vec3 to hold samples.
  //////////////////////////////////////////////////////////////////////////////
  std::array<QVector3D,64> m_ssaoKernel;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief random generator.
  //////////////////////////////////////////////////////////////////////////////
  std::default_random_engine generator;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief array of vec3 to hold random noise from randomKernel.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<GLfloat> m_ssaoNoise;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  GLuint m_noiseTexture;


  //////////////////////////////////////////////////////////////////////////////
  /// @brief QTimer clocks schedule updates for repainting the scene
  //////////////////////////////////////////////////////////////////////////////
  QTimer m_timer;


  //////////////////////////////////////////////////////////////////////////////
  /// @brief Loading all matricies to shader.
  //////////////////////////////////////////////////////////////////////////////
  void loadMatrixToShader();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Initialize all matricies.
  //////////////////////////////////////////////////////////////////////////////
  void initializeMatrix();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Method for all light calculations and sending light info to shader.
  //////////////////////////////////////////////////////////////////////////////
  void loadLightToShader();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Method for loading material defined in Material class to shader.
  //////////////////////////////////////////////////////////////////////////////
  void loadMaterialToShader();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Method for loading material defined in Material class to shader.
  //////////////////////////////////////////////////////////////////////////////
  void loadCameraInfoToShader();



  //////////////////////////////////////////////////////////////////////////////
  /// @brief Initializes a screen quad for deferred shading.
  //////////////////////////////////////////////////////////////////////////////
  void prepareQuad();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Initializes particle system geometry.
  //////////////////////////////////////////////////////////////////////////////
  void prepareParticles();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draws a quad object.
  //////////////////////////////////////////////////////////////////////////////
  void drawQuad();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draws particles.
  //////////////////////////////////////////////////////////////////////////////
  void drawParticles();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draws the links on top of everything
  //////////////////////////////////////////////////////////////////////////////
  void drawLinks();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Sets up a frame buffer object and initializes it.
  //////////////////////////////////////////////////////////////////////////////
  void setupFBO();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Method to calculate a sample kernel.
  //////////////////////////////////////////////////////////////////////////////
  void sampleKernel();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Method to create random noise.
  //////////////////////////////////////////////////////////////////////////////
  void randomKernel();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Creates and initialises lights and adds them to the list of
  /// objects.
  //////////////////////////////////////////////////////////////////////////////
  void setupLights();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Generates OpenGL vertex data for a sphere of _num_subdivisions
  /// subdivisions. It will get stored in m_sphere_data.
  /// @param[in] _num_subdivisions Recursion level for icosahedra subdivision.
  //////////////////////////////////////////////////////////////////////////////
  void generateSphereData(uint _num_subdivisions);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Now it selects randomly a particle, it splits it and advances them.
  //////////////////////////////////////////////////////////////////////////////
  void updateParticleSystem();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Sends updated particle information to OpenGL.
  //////////////////////////////////////////////////////////////////////////////
  void sendParticleDataToOpenGL();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Updates particle system model matrix.
  //////////////////////////////////////////////////////////////////////////////
  void updateModelMatrix();



  //////////////////////////////////////////////////////////////////////////////
  /// @brief Handle the key pressing events.
  /// @param[in] ev Event.
  //////////////////////////////////////////////////////////////////////////////
  void keyPressEvent(QKeyEvent * ev);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Handle the key pressing events.
  /// @param[in] key Event.
  //////////////////////////////////////////////////////////////////////////////
  void keyReleaseEvent(QKeyEvent *key);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Handle the mouse moving events.
  /// @param[in] event Event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseMoveEvent(QMouseEvent *event);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Handle the mouse pressing events.
  /// @param[in] event Event.
  //////////////////////////////////////////////////////////////////////////////
  void mousePressEvent(QMouseEvent *event);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Handle the mouse release events.
  /// @param[in] event Event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent *event);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Handle the mouse wheel events
  /// @param[in] event Event.
  //////////////////////////////////////////////////////////////////////////////
  void wheelEvent(QWheelEvent *event);



  GLuint VertexArrayID;










public slots:

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for changing particle size
  /// @param[in] _size size of particle.
  //////////////////////////////////////////////////////////////////////////////
  void setParticleSize(double _size);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for changing particle type
  /// @param[in] _type type of particle.
  //////////////////////////////////////////////////////////////////////////////
  void setParticleType(int _type);

  //not implemented yet
  void cancle();

  //not implemented yet
  void showConnections(bool _state);

  //not implement yet
  void setShading(QString _type);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for changing state of forces
  /// @param[in] _state state of forces
  //////////////////////////////////////////////////////////////////////////////
  void toggleForces(bool _state);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for changing cohesion amount
  /// only vallied for Linked Particles
  /// @param[in] _amount amount of cohesion.
  //////////////////////////////////////////////////////////////////////////////
  void setCohesion(int _amount);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot that triggers bulge
  //////////////////////////////////////////////////////////////////////////////
  void bulge();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for changing sprint amount
  /// only vallied for Linked Particles
  /// @param[in] _amount amount of spring.
  //////////////////////////////////////////////////////////////////////////////
  void setSpring(int _amount);
  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for the length of the branches in Growth Particles
  /// @param[in] _amount length of branch.
  //////////////////////////////////////////////////////////////////////////////
  void setBranchLength(double _amount);

  //not implemented yet
  void setGrowthRadius(int _amount);

  //not implemented yet
  void restart();

  //not implemented yet
  void setSplitType(QString _type);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief slot for changing amount of children per Growth Particle
  /// @param[in] _amount amount of children.
  //////////////////////////////////////////////////////////////////////////////
  void setChildThreshold(int _amount);





signals:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emit when particle type needs to be reset
  /// @param[in] indexNumber index number that selects the right Particle.
  //////////////////////////////////////////////////////////////////////////////
  void resetParticleType(int indexNumber);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when particle size needs to be reset
  /// @param[in] value size of particle.
  //////////////////////////////////////////////////////////////////////////////
  void resetParticleSize(double value);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when particle tap needs to be reset
  /// @param[in] index index of tap.
  //////////////////////////////////////////////////////////////////////////////
  void resetParticleTap(int index);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when forces needs to be reset
  /// @param[in] state state that forces is set to.
  //////////////////////////////////////////////////////////////////////////////
  void resetForces(bool state);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when cohesion value needs to be reset
  /// @param[in] value cohesion amount.
  //////////////////////////////////////////////////////////////////////////////
  void resetCohesion(int value);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when spring value needs to be reset
  /// @param[in] value spring amount.
  //////////////////////////////////////////////////////////////////////////////
  void resetSpring(int value);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when child threshold for growth particle needs to be reset
  /// @param[in] value Child threshold value.
  //////////////////////////////////////////////////////////////////////////////
  void resetChildrenThreshold(int value);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when branch Length needs to be reset
  /// @param[in] value length of branches.
  //////////////////////////////////////////////////////////////////////////////
  void resetBranchLength(double value);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when particles tap needs to be enabled/disabled
  /// @param[in] value state that tap is set to.
  //////////////////////////////////////////////////////////////////////////////
  void enableGrowthParticle(bool value);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Singal emits when particles tap needs to be enabled/disabled
  /// @param[in] value state that tap is set to.
  //////////////////////////////////////////////////////////////////////////////
  void enableLinkedParticle(bool value);


  //add additional resets here



};

#endif // WINDOW_H
