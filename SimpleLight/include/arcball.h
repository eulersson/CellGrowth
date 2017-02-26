#ifndef ARCBALLCAMERA_H
#define ARCBALLCAMERA_H

#pragma once

// Std. Includes
#include <vector>


// QT
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>




// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum ARCCamera_Movement {
    ARCFORWARD,
    ARCBACKWARD,
    ARCLEFT,
    ARCRIGHT
};


// Default camera values
const GLfloat ARCYAW        =  -90.0f;
const GLfloat ARCPITCH      =  0.0f;
const GLfloat ARCSPEED      =  0.2f;
const GLfloat ARCSENSITIVTY =  0.25f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class ArcBallCamera
{
public:
    // Camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // Eular Angles
    GLfloat Yaw; // Left and right
    GLfloat Pitch; // Up and down
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Dist;

    // Constructor with vectors
    ArcBallCamera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), GLfloat dist=15, QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), GLfloat yaw = ARCYAW,
           GLfloat pitch = ARCPITCH) :
        Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(ARCSPEED), MouseSensitivity(ARCSENSITIVTY)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->Dist = dist;
    }



    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    QMatrix4x4 GetViewMatrix()
    {
        // Create and instantiate matrix
        QMatrix4x4 view;
        view.setToIdentity();

        QVector3D center(0,0,2); //(this->Position + this->Front)



        // Update position could probably happen in its own function, and only get updated after an input event
        Position = QVector3D(cos(qDegreesToRadians(this->Yaw)) * cos(qDegreesToRadians(this->Pitch)),
                         -sin(qDegreesToRadians(this->Pitch)),
                         sin(qDegreesToRadians(this->Yaw)) * cos(qDegreesToRadians(this->Pitch))
                         );

        Position*=this->Dist;

        //this->Front = front.normalized();
        // Also re-calculate the Right and Up vector
        QVector3D right   = QVector3D::crossProduct(this->Front,this->WorldUp);
        QVector3D up      = QVector3D::crossProduct(this->Right,this->Front);
        this->Right       = right.normalized();
        this->Up          = up.normalized();




        Position+=center;
        //center = (this->Position + this->Front);
        view.lookAt(this->Position, center , this->Up);
        return view;
    }




    QVector3D getPosition() { return Position; }


    GLfloat getPitch() { return this->Pitch; }

    void SetDistance(GLfloat velocity)
    {
        this->Dist+=velocity;
        // Clamp value at zero
        if(this->Dist<0.1)
            this->Dist=0.1;

    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(ARCCamera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == ARCFORWARD) {
            SetDistance(-velocity);
        }


        if (direction == ARCBACKWARD) {
            SetDistance(velocity);

        }
        if (direction == ARCLEFT) {
        }
        if (direction == ARCRIGHT) {
        }

    }



    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;


        this->Yaw   += xoffset;
        this->Pitch += yoffset;


        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(int steps)
    {
        GLfloat deltaTime=1.0f;
        GLfloat offset=-(steps*0.02);

        GLfloat velocity = this->MovementSpeed * offset * deltaTime;

        SetDistance(velocity);
    }

};

#endif
