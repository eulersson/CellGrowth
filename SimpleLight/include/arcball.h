#ifndef ARCBALLCAMERA_H
#define ARCBALLCAMERA_H

// Std. Includes
#include <vector>


// QT
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QtCore/QtMath>



///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file arcball.h
/// @author Glenn Nygard
/// @version 0.0.1
/// @class ArcBallCamera
/// @brief Camera rotating around a given point.
///
/// Camera class based on the LearnOpenGL camera class, found here:
/// https://learnopengl.com/#!Getting-started/Camera. Modified to use QT
/// functions instead og GLM and to rotate around a central point (ArcBall)
/// instead of a 'flying man' approach.
///
/// Movement:
/// - alt + left mouse button to rotate around the rotation point
/// - scroll wheel or w/s to zoom
/// - middle mouse button to strafe (not added yet)
///////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Defines several possible options for camera movement.
///////////////////////////////////////////////////////////////////////////////////////////////////////
enum ARCCamera_Movement {
    ARCFORWARD,
    ARCBACKWARD,
    ARCLEFT,
    ARCRIGHT
};


/// Default camera values
///
///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Default YAW value.
///////////////////////////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCYAW        =  -90.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Default PITCH value.
///////////////////////////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCPITCH      =  0.0f;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Default SPEED value.
///////////////////////////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCSPEED      =  0.002f;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Default SENSITIVITY.
///////////////////////////////////////////////////////////////////////////////////////////////////////
const GLfloat ARCSENSITIVTY =  0.25f;


class ArcBallCamera
{
private:
  /// Camera Attributes
  ///
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera position.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector4D Position;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera front vector. The facing direction of the camera.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D Front;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera up vector. Tells the camera what direction is camera-up.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D Up;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera right vector. Tells the camera what direction is camera-right.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D Right;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Up vector. Tells the camera what direction is scene-up.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D WorldUp;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera rotation point. Tells the camera where in the scene the rotation focus is.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D rotationPoint;

  /// Eular Angles
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera yaw vector. Left and right rotation.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  GLfloat Yaw;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera pitch vector. Up and down rotation.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  GLfloat Pitch;

  /// Camera options
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera movement speed.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  GLfloat MovementSpeed;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Camera mouse speed/sensitivity.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  GLfloat MouseSensitivity;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Distance from the focus point.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D Dir;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief View matrix.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 view;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Calculates a new distance to the rotation point based on a velocity input.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  void Move(QVector3D velocity)
  {
    QMatrix4x4 tm;
    tm=view;
    tm.translate(velocity);

    this->Position=QVector3D(tm(0,3), tm(1,3), tm(2,3));
    Position=Position*view;


    QVector3D dir = QVector3D(Position.x(), Position.y(), Position.z())+rotationPoint;
    float dist = dir.length();


    if(dist>2)
    {
      view.translate(velocity);

    }
  }


public:


  /////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @param[in] position   Position of the camera.
  /// @param[in] dist       Camera distance from rotation point.
  /// @param[in] up         Up vector of camera.
  /// @param[in] yaw        Yaw vector of camera.
  /// @param[in] pitch         Pitch vector of camera.
  /// @brief Camera constructor setting all initial parameters. All parameters have default values.
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  ArcBallCamera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D dist=QVector3D(0,0,-1),
                QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), GLfloat yaw = ARCYAW, GLfloat pitch = ARCPITCH) :
      Front(QVector3D(0.0f, 0.0f, -1.0f)),
      MovementSpeed(ARCSPEED),
      MouseSensitivity(ARCSENSITIVTY),
      rotationPoint(0,0,0)
  {
    this->Position = QVector4D(position.x(), position.y(), position.z(), 1);
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->Dir = dist;

    view.setToIdentity();
    view.translate(this->Dir*20);

  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the view matrix calculated using Eular Angles and the LookAt Matrix
  ///////////////////////////////////////////////////////////////////////////////////////////////
  QMatrix4x4 GetViewMatrix()
  {
    return view;

  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the current position of the camera.
  ///////////////////////////////////////////////////////////////////////////////////////////////
  QVector3D getPosition() { return QVector3D(Position.x(), Position.y(), Position.z()); }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the current pitch of the camera.
  ///////////////////////////////////////////////////////////////////////////////////////////////
  GLfloat getPitch() { return this->Pitch; }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Sets the point the camera rotates around.
  ///////////////////////////////////////////////////////////////////////////////////////////////
  void setRotationPoint(QVector3D rp) { rotationPoint=rp; }


  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Processes keyboard inputs. Moves camera in a given direction.
  ///////////////////////////////////////////////////////////////////////////////////////////////
  void ProcessKeyboard(ARCCamera_Movement direction, GLfloat deltaTime)
  {
      GLfloat velocity = this->MovementSpeed * deltaTime;
      if (direction == ARCFORWARD) {
          this->Dir=this->Up;
      }
      if (direction == ARCBACKWARD) {
          //SetDistance(velocity);
          this->Dir=-this->Up;

      }
      if (direction == ARCLEFT) {
          this->Dir=-this->Right;
      }
      if (direction == ARCRIGHT) {
          this->Dir=this->Right;
      }

      //Position+=this->Dir*1;
      QVector3D vel = this->Dir*0.1;
      view.translate(vel);

      this->Position=QVector3D(view(0,3), view(1,3), view(2,3));

  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Processes input received from a mouse input system.
  /// Expects the offset value in both the x and y direction.
  ///////////////////////////////////////////////////////////////////////////////////////////////
  void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
  {
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (constrainPitch)
//    {
//        if (this->Pitch+yoffset > 89.8f)
//            yoffset=0;//this->Pitch = 89.0f;
//        if (this->Pitch+yoffset < -89.8f)
//            yoffset=0;//this->Pitch = -89.0f;
//    }
    this->Yaw   += xoffset;
    this->Pitch += yoffset;

//    QVector3D tmppos=Position;
//    Position = QVector3D (
//          cos(qDegreesToRadians(this->Yaw))* cos(qDegreesToRadians(this->Pitch)),
//         -sin(qDegreesToRadians(this->Pitch)),
//          sin(qDegreesToRadians(this->Yaw))* cos(qDegreesToRadians(this->Pitch))
//    );


////      Position = QVector3D (
////            (cos((this->Yaw))    *tmppos.x()) - (sin((this->Yaw))  *tmppos.z()),// + (sin((this->Pitch))*tmppos.y()),
////            (-sin((this->Pitch)) *tmppos.y() - cos((this->Pitch)) *tmppos.z()),
////            (sin((this->Yaw))    *tmppos.x()) + (cos((this->Yaw))  *tmppos.z())// + (sin((this->Pitch))*tmppos.y())
////                          );

////      Position = QVector3D (
////            Position.x()*tmppos.x(),
////            Position.y(),
////            Position.z()*tmppos.z()
////                          );

//      //Position+=tmppos;





//    // Also re-calculate the Right and Up vector
//    QVector3D right   = QVector3D::crossProduct(this->Front,this->WorldUp);
//    QVector3D up      = QVector3D::crossProduct(this->Right,this->Front);
//    this->Right       = right.normalized();
//    this->Up          = up.normalized();

//    view.lookAt(this->Position, rotationPoint , this->Up);


    // Also re-calculate the Right and Up vector
//    QVector3D right   = QVector3D::crossProduct(this->Front,this->WorldUp);
//    QVector3D up      = QVector3D::crossProduct(this->Right,this->Front);
//    this->Right       = right.normalized();
//    this->Up          = up.normalized();

    this->Position=QVector3D(view(0,3), view(1,3), view(2,3));

    //Position=QVector3D(1,1,1);

    // Do x rotation.
    view.rotate(xoffset, 0,1,0);
    // Get updated position by multiplying with the view matrix.
    Position=Position*view;
    QVector3D dir=(QVector3D(Position.x(), Position.y(), Position.z())-rotationPoint);
    // Calculate new camera right.
    this->Right=-QVector3D::crossProduct(dir,this->WorldUp);
    this->Right.normalize();
    // Do y rotation.
    view.rotate(yoffset, this->Right);
    //Position=Position*view;
    dir=(QVector3D(Position.x(), Position.y(), Position.z())-rotationPoint);
    // Calculate camera up.
    this->Up=-QVector3D::crossProduct(dir,this->Right);
    this->Up.normalize();

    //qDebug()<<QString::number(Position.z());
    //qDebug()<<QString::number(view(2,3));
//    qDebug()<<QString::number(Position.y());
//    qDebug()<<QString::number(Position.z());

  }


  void Refocus()
  {
    view.translate(-QVector3D(Position.x(), Position.y(), Position.z()));

    QVector3D dir=(QVector3D(Position.x(), Position.y(), Position.z())-rotationPoint);
    dir.normalize();
    view(2,0)=-dir.x();
    view(2,1)=-dir.y();
    view(2,2)=-dir.z();

//    view(1,0)=Up.x();
//    view(1,1)=Up.y();
//    view(1,2)=Up.z();

    this->Right=QVector3D::crossProduct(this->Up,dir);
    view(0,0)=this->Right.x();
    view(0,1)=this->Right.y();
    view(0,2)=this->Right.z();

    view.translate(QVector3D(Position.x(), Position.y(), Position.z()));

//    view.setToIdentity();
//    view.lookAt(QVector3D(Position.x(), Position.y(), Position.z()), rotationPoint , this->WorldUp);
//    qDebug()<<QString::number(view(2,3));
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Processes input received from a mouse scroll-wheel event.
  /// Only requires input on the vertical wheel-axis.
  ///////////////////////////////////////////////////////////////////////////////////////////////
  void ProcessMouseScroll(int steps)
  {
      GLfloat deltaTime=1.0f;
      GLfloat offset=-(steps*.8);
      QVector3D velocity = this->MovementSpeed * offset * deltaTime * (QVector3D(Position.x(), Position.y(), Position.z())
                                                                       -rotationPoint);
      Move(velocity);


  }

};

#endif
