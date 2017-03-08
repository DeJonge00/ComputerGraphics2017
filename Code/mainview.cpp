#include "mainview.h"
#include "math.h"

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    delete cubeModel;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &TBO);

    glDeleteTextures(1, &texPtr);

    vertices.clear();

    // Free the main shader
    delete mainShaderProg;

    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

/**
 * @brief MainView::createShaderPrograms
 *
 * Creates the shaderprogram for OpenGL rendering
 */
void MainView::createShaderPrograms() {
    // Qt wrapper (way cleaner than using pure OpenGL)
    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");
    mainShaderProg->link();

    // Store the locations (pointers in gpu memory) of uniforms in Glint's
    mainShaderProg->bind();
    shaderModel = glGetUniformLocation(mainShaderProg->programId(), "model");
    shaderView = glGetUniformLocation(mainShaderProg->programId(), "view");
    shaderProjection = glGetUniformLocation(mainShaderProg->programId(), "projection");
    shaderNormal = glGetUniformLocation(mainShaderProg->programId(), "normal");
    shaderMatColor = glGetUniformLocation(mainShaderProg->programId(), "materialColor");
    shaderComponents = glGetUniformLocation(mainShaderProg->programId(), "phongComponents");
    shaderPosition = glGetUniformLocation(mainShaderProg->programId(), "position");
    shaderLightPos = glGetUniformLocation(mainShaderProg->programId(), "lightPosition");
    shaderLightColor = glGetUniformLocation(mainShaderProg->programId(), "lightColor");
    shaderEyePos = glGetUniformLocation(mainShaderProg->programId(), "eyePosition");
    shaderSampler = glGetUniformLocation(mainShaderProg->programId(), "sampler");
}

/**
 * @brief MainView::createBuffers
 *
 * Creates necessary buffers for your application
 */
void MainView::createBuffers() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

    glGenBuffers(1,&CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

    glGenBuffers(1,&NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);

    glGenBuffers(1,&TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,0);

    glBindVertexArray(0);
}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    cubeModel = new Model(filename);
    numTris = cubeModel->getNumTriangles();

    Q_UNUSED(bufferObject);

    vertices = cubeModel->getVertices();
    normals = cubeModel->getNormals();
    vertexNumber = vertices.size();
    textureCoords = cubeModel->getTextureCoords();

    srand (static_cast <unsigned> (time(0)));
    for(int i = 0; i < vertexNumber / 3; i++) {
        QVector3D color {
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
            static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
        };
        colors.append(color);
        colors.append(color);
        colors.append(color);
    }

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(QVector3D), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,CBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(QVector3D), colors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,NBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(QVector3D), normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,TBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(QVector2D), textureCoords.data(), GL_STATIC_DRAW);
}

void MainView::loadTexture(QString file, GLuint texPtr) {
    QImageReader imgreader(file);
    QImage img = imgreader.read();
    QVector<quint8> bytes = imageToBytes(img);
    glBindTexture(GL_TEXTURE_2D, texPtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void MainView::updateBuffers() {
    // Change the data inside buffers (if you want)
    // make sure to change GL_STATIC_DRAW to GL_DYNAMIC_DRAW
    // in the call to glBufferData for better performance

}


void MainView::updateUniforms() {
    // TODO: update the uniforms in the shaders using the glUniform<datatype> functions

}

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    //glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    createShaderPrograms();

    createBuffers();

    eye = QVector3D {2,2,10};
    viewDirection = - eye;
    loadModel(":/models/cube.obj", cubeBO);

    glGenTextures(1,&texPtr);
    loadTexture(":/textures/rug_logo.png", texPtr);

    // For animation, you can start your timer here

}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {

    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    updateCameraPosition();
    updateMatrices();

    // Clear the screen before rendering
    glClearColor(0.5f,0.5f,0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShaderProg->bind();

    glBindVertexArray(VAO);
    renderRaytracerScene();
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPtr);

    mainShaderProg->release();
}

void MainView::updateMatrices() {
    view.setToIdentity();
    projection.setToIdentity();

    if (viewMode == 0) {
        view.lookAt(eye, QVector3D {0, 0, 0}, QVector3D {0, 1, 0});
    } else {
        view.lookAt(eye, eye + viewDirection, QVector3D {0, 1, 0});
    }
    projection.perspective(30.0f, (float)width() / (float)height(), 0.1f, 8000.0f);
    model = rotation * scaling;
    normal = rotation;

    mainShaderProg->bind();
    glUniformMatrix4fv(shaderModel, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(shaderView, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(shaderProjection, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(shaderNormal, 1, GL_FALSE, normal.data());
    glUniform3f(shaderEyePos, eye[0], eye[1], eye[2]);
}
