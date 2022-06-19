#include "openglpicture.h"
#include <cmath>

OpenGLPicture::OpenGLPicture(bool isAnimated, double chosenT, QWidget *parent) :
        QOpenGLWidget(parent),
        isAnimated(isAnimated),
        chosenTime(chosenT) {
}

OpenGLPicture::~OpenGLPicture() {
}

int OpenGLPicture::getPeriodSize() const {
    return periodSize;
}

void OpenGLPicture::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();

    /** Enable depth buffer */
    glEnable(GL_DEPTH_TEST);

    geometries = std::unique_ptr<GeometryEngine>(new GeometryEngine(this, periodSize));

    timer.start(0, this);
}

void OpenGLPicture::initShaders() {
    /** Compile vertex shader */
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl")) {
        close();
    }
    /** Compile fragment shader */
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl")) {
        close();
    }
    /** Link shader pipeline */
    if (!program.link()) {
        close();
    }
    /** Bind shader pipeline for use */
    if (!program.bind()) {
        close();
    }
}

void OpenGLPicture::timerEvent(QTimerEvent *) {
    curTime += 0.05;
    update();
}

void OpenGLPicture::resizeGL(int w, int h) {
    /** Calculate aspect ratio */
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    /** Set near plane to 0.5, far plane to 7.0, field of view 70 degrees */
    const qreal zNear = 0.5, zFar = 100.0, fov = 70.0;

    /** Reset projection */
    projection.setToIdentity();

    /** Set perspective projection */
    projection.perspective(fov, static_cast<float>(aspect), zNear, zFar);
}

void OpenGLPicture::paintGL() {
    /** Clear color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /** Calculate model view transformation */
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -2.5f);
    matrix.rotate(static_cast<float>(periodSize * curTime), rotationAxis.x(),
                  rotationAxis.y(), rotationAxis.z());
    /** Set modelview-projection matrix */
    program.setUniformValue("mvp", projection * matrix);

    program.setUniformValue("colour", color);

    if (isAnimated) {
        program.setUniformValue("time", static_cast<GLfloat>(curTime));
    } else {
        program.setUniformValue("time", static_cast<GLfloat>(chosenTime));
    }

    geometries->drawGeometry(&program);
}

void OpenGLPicture::setChosenTime(int t) {
    isAnimated = false;
    auto mid = periodSize / 2;
    chosenTime = M_PI * (t - mid) / periodSize;
    update();
}

void OpenGLPicture::setColor(const QColor &newColor) {
    color = newColor;
    update();
}

void OpenGLPicture::setRotationAxis(const QString &coordinatesText) {
    auto axisCoordinates = coordinatesText.split(" ");
    if (axisCoordinates.size() == 3) {
        rotationAxis = QVector3D(axisCoordinates[0].toFloat(),
                                 axisCoordinates[1].toFloat(),
                                 axisCoordinates[2].toFloat());
        update();
    }
}

void OpenGLPicture::setAnimated(bool newIsAnimated) {
    isAnimated = newIsAnimated;
    update();
}
