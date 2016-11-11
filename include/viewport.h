#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

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
