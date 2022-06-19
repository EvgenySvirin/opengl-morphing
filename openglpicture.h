#ifndef OPENGLPICTURE_H
#define OPENGLPICTURE_H

#include "geometryengine.h"
#include <QOpenGLWidget>
#include <QBasicTimer>
#include <QColor>
#include <memory>


class OpenGLPicture : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
    OpenGLPicture(bool isAnimate = false,
                double chosenTime = 0.0,
                QWidget *parent = nullptr);

    ~OpenGLPicture() override;

    int getPeriodSize() const;

public slots:
    void setChosenTime(int t);

	void setColor(const QColor &);

    void setRotationAxis(const QString &);

    void setAnimated(bool value = true);

protected:
	void timerEvent(QTimerEvent *e) override;

	void initializeGL() override;

	void paintGL() override;

	void resizeGL(int w, int h) override;

	void initShaders();

private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    std::unique_ptr<GeometryEngine> geometries;
    QMatrix4x4 projection;
    QVector3D rotationAxis = {1.0, 1.0, 1.0};
    QColor color = QColor::fromRgb(100.0, 200.0, 100.0);

    int periodSize = 100;
    double curTime = 0.;
    bool isAnimated = true;
    double chosenTime = 0.0;
};

#endif /** OPENGLPICTURE_H */
