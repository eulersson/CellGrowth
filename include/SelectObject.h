////////////////////////////////////////////////////////////////////////////////
/// @file SelectObject.h
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#ifndef SELECTOBJECT_H
#define SELECTOBJECT_H



// QT
//#include <QtGui/QOpenGLFunctions>
#include <QVector3D>
#include <QDebug>

// OpenGL
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>




////////////////////////////////////////////////////////////////////////////////
/// @class SelectObject
/// @brief Abstract class for scene objects.
///
/// This class will work as an abstract class containing all the objects of the
/// scene that will require key/mouse inputs. It will also provide camera
/// calculations for these objects.
////////////////////////////////////////////////////////////////////////////////
class SelectObject
{

public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief Compare a colour to the current unique colour for the object.
  /// @param _colour Colour to be compared.
  //////////////////////////////////////////////////////////////////////////////
  virtual int compareUniqueColour(QVector3D _colour) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Create the object geometry.
  /// @param[in] context Scene context
  /// @param[out] masterUniqueColour The updated unique colour to be used as object identifier.
  //////////////////////////////////////////////////////////////////////////////
  virtual void createGeometry(QVector3D &masterUniqueColour) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draw the object to the main buffer.
  //////////////////////////////////////////////////////////////////////////////
  virtual void draw()=0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Draw the object to a back buffer using the objects unique colour.
  //////////////////////////////////////////////////////////////////////////////
  virtual void drawBackBuffer()=0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Process mouse movement and apply necessary transformations.
  /// @param offsetx X movement since last update.
  /// @param offsety Y movement since last update.
  /// @param offsetz Z movement since last update.
  //////////////////////////////////////////////////////////////////////////////
  virtual void processMouseMovement(float _offsetx,
                                    float _offsety,
                                    float _offsetz,
                                    QVector3D _campos,
                                    QMatrix4x4 _view) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Set object as currently clicked/not clicked.
  /// @param uColour Unique colour to be checked.
  /// @param state State the object should take (clicked or unclicked)
  //////////////////////////////////////////////////////////////////////////////
  virtual void setClicked(QVector3D uColour, bool state) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Indicate mouse is hovering over the object.
  //////////////////////////////////////////////////////////////////////////////
  virtual void setHover(int id) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the position of the object.
  //////////////////////////////////////////////////////////////////////////////
  virtual QVector3D getPosition() = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns the main shader of the object.
  //////////////////////////////////////////////////////////////////////////////
  virtual void getMainProgram(QOpenGLShaderProgram **retshader) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns a new single unique colour, incrementing the colour variable.
  //////////////////////////////////////////////////////////////////////////////
  QVector3D getNewUniqueColour(QVector3D &masterUniqueColour)
  {
      masterUniqueColour=QVector3D(masterUniqueColour.x()+1, masterUniqueColour.y(), masterUniqueColour.z());
      return masterUniqueColour;
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief Returns multiple new unique colours, and increments the colour variable accordingly.
  //////////////////////////////////////////////////////////////////////////////
  std::vector<QVector3D> getMultipleNewUniqueColour(int amount, QVector3D &masterUniqueColour)
  {
      std::vector<QVector3D> uColourVec;
      for(int i=0;i<amount;i++)
      {
          uColourVec.push_back(getNewUniqueColour(masterUniqueColour));
      }

      return uColourVec;
  }

};


#endif
