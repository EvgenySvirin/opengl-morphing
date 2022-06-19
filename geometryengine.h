#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : public QObject, protected QOpenGLFunctions {
    Q_OBJECT

public:
    GeometryEngine(QObject *parent = nullptr, int periodSize = 100);

    virtual ~GeometryEngine();

    void drawGeometry(QOpenGLShaderProgram *program);
    int getPeriodSize() const;

private:
    int periodSize;

    void initGeometry();
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif /** GEOMETRYENGINE_H */
