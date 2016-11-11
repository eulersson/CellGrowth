/*
 * You won't be using the Viewport class, instead you will need to
 * subclass it and override the render() and initialize() functions.
 * The viewport class will allow us Qt to be handling all the OpenGL
 * functionality. So the preparation and initialization is kept simple.
 * Also notice that we will be using OpenGL ES, it's the one that Qt
 * supports.
 *
 * If you want to understand what is going on check out this Qt
 * tutorial, I basically followed the steps to get the Viewport
 * class:
 *
 *   http://doc.qt.io/qt-5/qtgui-openglwindow-example.html
 * */

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWindow>
#include <QOpenGLFunctions>

class QPainter;
class QOpenGLFunctions;
class QOpenGLPaintDevice;

class Viewport : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Viewport(QWindow *parent = 0);
    ~Viewport();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_update_pending;
    bool m_animating;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif // VIEWPORT_H
