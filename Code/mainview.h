#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <ctime>
#include <cstdlib>
#include <QImage>
#include <QImageReader>

#define pi 3.141592653589793238

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

    void updateRotation(int x, int y, int z);
    void updateModel(QString name);
    void updateShader(QString name);
    void updateScale(float scale);

    //TODO: decide what should be public and what not
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector3D> colors;
    int vertexNumber;

    QVector3D convertHSLtoRGB(float H, float S, float L);

    void loadTexture(QString file, GLuint texPtr);
    QVector<quint8> imageToBytes(QImage image);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private:
    QOpenGLDebugLogger *debugLogger;

    void createShaderPrograms();
    void createBuffers();
    void updateBuffers();
    void updateUniforms();
    void updateMatrices();
    void updateCameraPosition();

    // Raytracer scene functions
    void renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos);
    void renderCube(QVector3D pos, QVector3D lightpos);
    void renderRaytracerScene();

    /* Add your private members below */
    void loadModel(QString filename, GLuint bufferObject);

    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg;

    QTimer timer; // timer used for animation

    Model *cubeModel;
    GLuint cubeBO;

    unsigned numTris;

    GLuint VAO, VBO, CBO, NBO, TBO;
    QMatrix4x4 model, view, projection, normal;
    QMatrix4x4 rotation, scaling;
    QVector3D currentRotation;

    int prevMouseX, prevMouseY;
    float currentScale = 1;

    bool forpressed, backpressed, leftpressed, rightpressed, uppressed, downpressed;
    QVector3D eye;

    GLint shaderModel, shaderView, shaderProjection, shaderNormal, shaderPosition;
    GLint shaderMatColor, shaderComponents, shaderLightPos, shaderLightColor, shaderEyePos;
    GLint shaderTexture, shaderSampler;

    GLuint texPtr;
    QVector<QVector2D> textureCoords;

    int viewMode = 0; //0=normal, 1=first person

    QVector3D viewDirection;
    int moveSpeed = 8;

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
