////////////////////////////////////////////////////////////////////////////////
/// @file ArcBallCamera.h
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#ifndef ARCBALLCAMERA_H
#define ARCBALLCAMERA_H

// Standard
#include <vector>

// Qt
#include <QOpenGLFunctions_4_5_Core>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>

////////////////////////////////////////////////////////////////////////////////
/// @enum ARCCamera_Movement
/// @brief Defines several possible options for camera movement.
/// @var ARCFORWARD
/// @var ARCBACKWARD
/// @var ARCLEFT
/// @var ARCRIGHT
////////////////////////////////////////////////////////////////////////////////
enum ARCCamera_Movement {
    ARCFORWARD,
    ARCBACKWARD,
    ARCLEFT,
    ARCRIGHT
};

////////////////////////////////////////////////////////////////////////////////
/// @brief Default YAW value.
////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCYAW = -90.0f;

////////////////////////////////////////////////////////////////////////////////
/// @brief Default PITCH value.
////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCPITCH = 0.0f;

////////////////////////////////////////////////////////////////////////////////
/// @brief Default SPEED value.
////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCSPEED = 0.002f;

////////////////////////////////////////////////////////////////////////////////
/// @brief Default SENSITIVITY.
////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCSENSITIVTY = 0.25f;

////////////////////////////////////////////////////////////////////////////////
/// @class ArcBallCamera
/// @brief Camera rotating around a given point.
///
/// Camera class based on the LearnOpenGL camera class, found here:
/// https://learnopengl.com/#!Getting-started/Camera. Modified to use Qt
/// functions instead og GLM and to rotate around a central point (ArcBall)
/// instead of a 'flying man' approach.
///
/// Movement:
/// - alt + left mouse button to rotate around the rotation point
/// - scroll wheel or w/s to zoom
/// - middle mouse button to strafe (not added yet)
////////////////////////////////////////////////////////////////////////////////
class ArcBallCamera
{

public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera constructor setting all initial parameters.
  /// All parameters have default values.
  /// @param _position   Position of the camera.
  /// @param _dist       Camera distance from rotation point.
  /// @param _up         Up vector of camera.
  /// @param _yaw        Yaw vector of camera.
  /// @param _pitch      Pitch vector of camera.
  //////////////////////////////////////////////////////////////////////////////
  ArcBallCamera(
      QVector3D _position = QVector3D(0.0f, 0.0f, 0.0f),
      QVector3D _dist = QVector3D(0,0,-1),
      QVector3D _up = QVector3D(0.0f, 1.0f, 0.0f),
      GLfloat _yaw = ARCYAW,
      GLfloat _pitch = ARCPITCH);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the view matrix calculated using Eular Angles and the
  /// LookAt Matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 getViewMatrix();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the current position of the camera.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D getPosition();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the current pitch of the camera.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat getPitch();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Sets the point the camera rotates around.
  //////////////////////////////////////////////////////////////////////////////
  void setRotationPoint(QVector3D rp);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes keyboard inputs. Moves camera in a given direction.
  //////////////////////////////////////////////////////////////////////////////
  void processKeyboard(ARCCamera_Movement direction, GLfloat deltaTime);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes input received from a mouse input system.
  /// Expects the offset value in both the x and y direction.
  //////////////////////////////////////////////////////////////////////////////
  void processMouseMovement(GLfloat xoffset, GLfloat yoffset,
                            GLboolean constrainPitch = true);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void refocus();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Processes input received from a mouse scroll-wheel event.
  /// Only requires input on the vertical wheel-axis.
  /// @param[in] steps !!!MISSING
  //////////////////////////////////////////////////////////////////////////////
  void processMouseScroll(int steps);

private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Calculates a new distance to the rotation point based on a velocity
  /// input.
  //////////////////////////////////////////////////////////////////////////////
  void move(QVector3D velocity);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera position.
  //////////////////////////////////////////////////////////////////////////////
  QVector4D m_position;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera front vector. The facing direction of the camera.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_front;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera up vector. Tells the camera what direction is camera-up.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_up;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera right vector. Tells camera what direction is camera-right.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_right;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Up vector. Tells the camera what direction is scene-up.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_worldUp;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera rotation point. Tells the camera where in the scene the
  /// rotation focus is.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_rotationPoint;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera yaw vector. Left and right rotation. Euler angle.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_yaw;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera pitch vector. Up and down rotation. Euler angle.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_pitch;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera movement speed.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_movementSpeed;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Camera mouse speed/sensitivity.
  //////////////////////////////////////////////////////////////////////////////
  GLfloat m_mouseSensitivity;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Distance from the focus point.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D m_dir;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief View matrix.
  //////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 m_view;

};

#endif // ARCBALLCAMERA_H
