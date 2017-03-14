////////////////////////////////////////////////////////////////////////////////
/// @file PointLight.h
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Project
#include "Manipulator.h"
#include "SelectObject.h"

////////////////////////////////////////////////////////////////////////////////
/// @brief Default position
////////////////////////////////////////////////////////////////////////////////
const QVector3D POSITION     = QVector3D(0.0f, 0.0f, 0.0f);

////////////////////////////////////////////////////////////////////////////////
/// @brief Default light colour
////////////////////////////////////////////////////////////////////////////////
const QVector3D LIGHT_COLOUR = QVector3D(0.8f, 0.8f, 0.8f);

////////////////////////////////////////////////////////////////////////////////
/// @class PointLight
/// @brief Deals with inputs, and diverts them to the appropriate classes. Point
/// light class. Stores functions and parameters related to the scene lights.
////////////////////////////////////////////////////////////////////////////////
class PointLight : public SelectObject
{
public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor.
  /// @param[in] _position !!!MISSING
  /// @param[in] _manipshaderp !!!MISSING
  /// @param[in] _sunshaderp !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  PointLight(
      QVector3D _position,
      QOpenGLShaderProgram *_manipshaderp,
      QOpenGLShaderProgram *_sunshaderp);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Compare a unique colour with custom object unique colour.
  /// @param[in] _colour !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  int compareUniqueColour(QVector3D _colour) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create light geometry.
  /// @param[in] context !!!MISSING
  /// @param[out] masterUniqueColour !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void createGeometry(QOpenGLContext *context, QVector3D &masterUniqueColour) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draw light object to main buffer.
  //////////////////////////////////////////////////////////////////////////////
  void draw() override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draw light object to back buffer using the object unique colour.
  //////////////////////////////////////////////////////////////////////////////
  void drawBackBuffer() override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Process mouse movements.
  /// @param[in] offsetx !!!MISSING
  /// @param[in] offsety !!!MISSING
  /// @param[in] offsetz !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void processMouseMovement(float offsetx, float offsety, float offsetz) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Set object as clicked.
  /// @param[in] uColour !!!MISSING
  /// @param[in] state !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void setClicked(QVector3D uColour, bool state) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Mark the object as being under the mouse cursor.
  /// @param[in] id !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void setHover(int id) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns light position.
  /// @returns !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  QVector3D getPosition() override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns light object manipulator.
  /// @returns !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  Manipulator getManipulator();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns main light shader program.
  /// @param[in] retshader !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void getMainProgram(QOpenGLShaderProgram **retshader);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Initialises VBO/VAO of light object.
  /// @param[in] _context !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void setupObject(QOpenGLContext *_context);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Updates the light model matrix for use with the camera.
  /// @param[in] offsetx !!!MISSING
  /// @param[in] offsety !!!MISSING
  /// @param[in] offsetz !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void updateModelMatrix(float offsetx, float offsety, float offsetz);

private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light manipulator.
  //////////////////////////////////////////////////////////////////////////////
  Manipulator m_manip;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light position.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_position;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light VBO.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLBuffer* m_vbo;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light VAO.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLVertexArrayObject *m_vao;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Stores the light representation position (the little icon
  /// indicating the light).
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_points[3];

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light representation shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_sunshaderp;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Manipulator shader program.
  //////////////////////////////////////////////////////////////////////////////
  QOpenGLShaderProgram *m_manipshaderp;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Light model matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_model;

};