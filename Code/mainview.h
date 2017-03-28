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
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <ctime>
#include <cstdlib>
#include <QImage>
#include <QImageReader>
#include "sceneobject.h"

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

    void loadTexture(QString file);
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
    void createTextures();
    void updateBuffers(int index);
    void updateTextures(int index);
    void updateUniforms();
    void updateMatrices();
    void updateCameraPosition();

    // Scene functions
    void renderModel(SceneObject a);
    void renderColoredModel(int modelIndex, QVector3D matColor, QVector3D pos, float size, QVector4D phong);
    void renderScene();
    void initializeScene();

    /* Add your private members below */
    void loadModel(QString filename, GLuint bufferObject);

    // Shader programs, GLint for uniforms/buffer objects, other variables
    QOpenGLShaderProgram *mainShaderProg, *shaderprog2;

    QTimer timer; // timer used for Animations
    long time = 0;

    QVector<Model *> models;
    QVector<QVector<quint8>> textures;
    QVector<int> texWidths, texHeights;
    GLuint cubeBO;

    unsigned numTris;

    GLuint VAO, VBO, CBO, NBO, TBO;
    GLuint VAO2, VBO2, UVBO;
    GLuint gBuffer1, gBuffer2, gBuffer3, FBO;
    GLint defaultFramebuffer;
    QMatrix4x4 model, view, projection, normal;
    QMatrix4x4 rotation, scaling;
    QVector3D currentRotation;

    int prevMouseX, prevMouseY;
    float currentScale = 0.1;

    bool forpressed, backpressed, leftpressed, rightpressed, uppressed, downpressed;
    QVector3D eye;

    GLint shaderModel, shaderView, shaderProjection, shaderNormal, shaderPosition, shaderSize;
    GLint shaderMatColor, shaderComponents, shaderLightPos, shaderLightColor, shaderEyePos;
    GLint shaderTexture, shaderSampler, shaderLightingOn, shaderCenterPos, shaderRotation;
    GLint shader2Diffuse, shader2Normal, shader2Depth;

    GLuint texPtr;
    QVector<QVector2D> textureCoords;

    int viewMode = 0; //0=normal, 1=first person

    QVector3D viewDirection;
    int moveSpeed = 8;

    QVector3D lightPos, lightCol;

    QVector<SceneObject*> sceneobjects;
    int centerIndex;


private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
