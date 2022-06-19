#include "geometryengine.h"
#include <QVector3D>

GeometryEngine::GeometryEngine(QObject *parent,
                               int periodSize):
    QObject(parent),
    periodSize(periodSize),
    indexBuf(QOpenGLBuffer::IndexBuffer) {
    initializeOpenGLFunctions();
    /** Generate 2 VBOs */
    arrayBuf.create();
    indexBuf.create();

    initGeometry();
}

int GeometryEngine::getPeriodSize() const {
    return periodSize;
}

GeometryEngine::~GeometryEngine() {
    arrayBuf.destroy();
    indexBuf.destroy();
}

void GeometryEngine::initGeometry() {
    QVector<QVector<QVector3D>> triangleFaces = {
    /** Vertex data for face 0 */
    {QVector3D(-1.0f, -1.0f,  1.0f),
     QVector3D( 1.0f, -1.0f,  1.0f),
     QVector3D(-1.0f,  1.0f,  1.0f)},

    /** Vertex data for face 1 */
    {QVector3D( 1.0f, -1.0f,  1.0f),
     QVector3D( 1.0f, -1.0f, -1.0f),
     QVector3D( 1.0f,  1.0f,  1.0f)},

    /** Vertex data for face 2 */
    {QVector3D( 1.0f, -1.0f, -1.0f),
     QVector3D(-1.0f, -1.0f, -1.0f),
     QVector3D( 1.0f,  1.0f, -1.0f)},

    /** Vertex data for face 3 */
    {QVector3D(-1.0f, -1.0f, -1.0f),
     QVector3D(-1.0f, -1.0f,  1.0f),
     QVector3D(-1.0f,  1.0f, -1.0f)},

    /** Vertex data for face 4 */
    {QVector3D(-1.0f, -1.0f, -1.0f),
     QVector3D( 1.0f, -1.0f, -1.0f),
     QVector3D(-1.0f, -1.0f,  1.0f)},

    /** Vertex data for face 5 */
    {QVector3D(-1.0f,  1.0f,  1.0f),
     QVector3D( 1.0f,  1.0f,  1.0f),
     QVector3D(-1.0f,  1.0f, -1.0f)}
    };
    for (auto& triangle : triangleFaces) {
        for (auto& vertex: triangle) {
            vertex /= 2.0f;
        }
    }

    QVector<QVector3D> vertices;
    QVector<GLushort> indices;
    auto index = 0;
    const float step = 1.f / static_cast<float>(periodSize);
    for (const auto& triangle : triangleFaces) {
        auto A = triangle[0];
        auto B = triangle[1];
        auto C = triangle[2];
        for (auto y = 0; y <= periodSize; ++y) {
            for (auto x = 0; x <= periodSize; ++x) {
                auto curVector = A;
                curVector += step * x * (B - A);
                curVector += step * y * (C - A);
                vertices.append(curVector);
                ++index;

                if (step / 2.f < y) { /** forget abot first line */
                    /**
                      * *-*( index-1 )-*( index )-*-*
                      * *-*(index-1-T)-*(index-T)-*-*
                      *
                      */

                    /**
                      * it seemes like redundant amount
                      * but it is actually not
                      */

                    indices.push_back(static_cast<unsigned short>(index));
                    indices.push_back(static_cast<unsigned short>(index - periodSize));
                    indices.push_back(static_cast<unsigned short>(index - periodSize + 1));
                    indices.push_back(static_cast<unsigned short>(index));
                    indices.push_back(static_cast<unsigned short>(index - 1));
                    indices.push_back(static_cast<unsigned short>(index - periodSize));
                }
            }
        }
    }

    /** Transfer vertex data to VBO 0 */
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(),
                      static_cast<int>(vertices.size()) *
                      static_cast<int>(sizeof(QVector3D)));

    /** Transfer index data to VBO 1 */
    indexBuf.bind();
    indexBuf.allocate(indices.data(),
                      static_cast<int>(indices.size()) *
                      static_cast<int>(sizeof(GLushort)));
}

void GeometryEngine::drawGeometry(QOpenGLShaderProgram *program) {
    /** Tell OpenGL which VBOs to use */
    arrayBuf.bind();
    indexBuf.bind();

    /** Offset for position */
    quintptr offset = 0;

    /** Tell OpenGL programmable pipeline how to locate vertex position data */
    int vertexLocation = program->attributeLocation("position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, static_cast<int>(offset),
                                3, sizeof(QVector3D));

    /** Draw cube geometry using indices from VBO 1 */
    auto count = static_cast<int>(indexBuf.size()) / static_cast<int>(sizeof(unsigned short));
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
}
