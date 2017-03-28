////////////////////////////////////////////////////////////////////////////////
/// @file InputManager.h
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#ifndef INPUT_H
#define INPUT_H

// Native
#include <memory>

// Qt
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QVector3D>
#include <QWindow>
#include <QOpenGLFramebufferObject>

// Project
#include "ArcBallCamera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SelectObject.h"

////////////////////////////////////////////////////////////////////////////////
/// @class InputManager
/// @brief Deals with inputs, and diverts them to the appropriate classes.
///
/// This class will mainly be dealing with movement and updates caused by user
/// input. It will maintain the Camera class and divert inputs to that, or to
/// SelectObject instances in the scene.
////////////////////////////////////////////////////////////////////////////////
class InputManager
{

public:
  QOpenGLFramebufferObject* m_fbo;
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Custom constructor taking a QWindow.
  /// @param[in] _window Window the InputManager is associated with.
  //////////////////////////////////////////////////////////////////////////////
  InputManager (QWindow *_window);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Run Hover function for all objects currently under mouse cursor.
  //////////////////////////////////////////////////////////////////////////////
  void onHover();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Moves/alters the camera positions based on user input.
  //////////////////////////////////////////////////////////////////////////////
  void doMovement();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Gets the unique colour at a given screen coordinate. This is the
  /// value written with drawBackBuffer in the SelectObject.
  /// @param[in] _x X screen coordinate.
  /// @param[in] _y Y screen coordinate.
  /// @param[in] _manipulatorProgram Program to be used to draw geomtry to back
  /// buffer.
  //////////////////////////////////////////////////////////////////////////////
  void getUniqueColour();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Runs getUniqueColour and compares them to the object unique
  /// colours. Any matching object is set to clicked.
  /// @param[in] _x !!!MISSING
  /// @param[in] _y !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void doSelection(const int _x, const int _y);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Add another shader program to the InputManager. These are used to
  /// calculate camera movement and to get unique colours.
  /// @param[in] _program Shader program to add.
  //////////////////////////////////////////////////////////////////////////////
  void addShaderProgram(QOpenGLShaderProgram* _program);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Setup main camera.
  //////////////////////////////////////////////////////////////////////////////
  void setupCamera();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Load the list of objects to the InputManager. These are the
  /// objects that will be considered for the InputManager calculations.
  /// @param[in] _objectList Object list to set.
  //////////////////////////////////////////////////////////////////////////////
  void setObjectList(std::vector<std::shared_ptr<SelectObject>> _objectList);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Gives the value of the current unique colour variable.
  /// @returns Current unique colour.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D getCurrentUniqueColour();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Sets the value of the current unique colour variable.
  /// @param[in] uc The colour to be set.
  //////////////////////////////////////////////////////////////////////////////
  void setCurrentUniqueColour(QVector3D _uc);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Get the current projection matrix for use with other calculations.
  /// \return The projection matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 getProjectionMatrix();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Get the current view matrix for use with other calculations.
  /// @returns The view matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 getViewMatrix();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Get the current camera position for use with shading calculations.
  /// @returns Camera position.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D getCameraPosition();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Calculate mouse click release.
  /// @param[in] event Event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent *_event);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes the mouse down event.
  /// @param[in] event Event.
  //////////////////////////////////////////////////////////////////////////////
  void mousePressEvent(QMouseEvent *_event);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes the mouse movements.
  /// @param[in] event Mouse event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseMoveEvent(QMouseEvent *_event);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Process key press events.
  /// @param[in] key Key event.
  //////////////////////////////////////////////////////////////////////////////
  void keyPressEvent(QKeyEvent *_key);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes key release.
  /// @param[in] key Key event.
  //////////////////////////////////////////////////////////////////////////////
  void keyReleaseEvent(QKeyEvent *_key);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes wheel events.
  /// @param[in] event Key event.
  //////////////////////////////////////////////////////////////////////////////
  void wheelEvent(QWheelEvent *_event);

private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Variable storing the current unique colour. This is being used
  /// throughout the class and is made global to not have to be recalculated.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_currentUniqueColour;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Window associated.
  //////////////////////////////////////////////////////////////////////////////
  QWindow *m_window;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Main scene camera. This should not leave the class.
  //////////////////////////////////////////////////////////////////////////////
  ArcBallCamera m_camera;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Scene view matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_view;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Scene projection matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_projection;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Vector of shader programs.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<QOpenGLShaderProgram*> m_programs;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Vector of selectobjects in the scene.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<std::shared_ptr<SelectObject>> m_objectList;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Array of bools stating which buttons are currently pressed.
  //////////////////////////////////////////////////////////////////////////////
  bool m_keys[1024];

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Because of issues with the alt key, it has its own variable
  /// (alt=special snowflake).
  //////////////////////////////////////////////////////////////////////////////
  bool m_alt_key=false;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Variable stating wether or not the mouse is currently pressed.
  //////////////////////////////////////////////////////////////////////////////
  bool m_mousePressed;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief GLfloat storing last position for screen x.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_lastX = 400;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief GLfloat storing last position for screen y.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_lastY = 300;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Z position of clicked item. A global variable to make sure the z
  /// position is only gotten on mouse click, not mouse down (continous).
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_clickZ;




};

#endif




