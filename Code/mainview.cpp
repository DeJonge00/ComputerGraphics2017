#include "mainview.h"
#include "cmath"

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
    for(int i = 0; i < models.size(); i++) {
        delete models[i];
    }
    models.clear();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &TBO);

    glDeleteTextures(1, &texPtr);

    vertices.clear();

    // Free the main shader
    delete mainShaderProg;
    delete shaderprog2;

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
    shaderSize = glGetUniformLocation(mainShaderProg->programId(), "size");
    shaderSampler = glGetUniformLocation(mainShaderProg->programId(), "sampler");
    shaderRotation = glGetUniformLocation(mainShaderProg->programId(), "rotation");
    mainShaderProg->release();

    shaderprog2 = new QOpenGLShaderProgram();
    shaderprog2->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader2.glsl");
    shaderprog2->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader2.glsl");
    shaderprog2->link();

    shaderprog2->bind();
    shader2Diffuse = glGetUniformLocation(shaderprog2->programId(), "diffuse");
    shader2Normal = glGetUniformLocation(shaderprog2->programId(), "normals");
    shader2Depth = glGetUniformLocation(shaderprog2->programId(), "depth");
    shader2Inverse = glGetUniformLocation(shaderprog2->programId(), "inverse");
    shader2LightPos = glGetUniformLocation(shaderprog2->programId(), "lightPosition");
    shader2LightColor = glGetUniformLocation(shaderprog2->programId(), "lightColor");
    shader2EyePos = glGetUniformLocation(shaderprog2->programId(), "eyePosition");
    shader2Components = glGetUniformLocation(shaderprog2->programId(), "phongComponents");
    shaderprog2->release();
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


    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1,&VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);

    glGenBuffers(1,&UVBO);
    glBindBuffer(GL_ARRAY_BUFFER, UVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);

    glBindVertexArray(0);



    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBuffer1, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBuffer2, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gBuffer3, 0);
    GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void MainView::createTextures() {
    glGenTextures(1, &gBuffer1);
    glBindTexture(GL_TEXTURE_2D, gBuffer1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &gBuffer2);
    glBindTexture(GL_TEXTURE_2D, gBuffer2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &gBuffer3);
    glBindTexture(GL_TEXTURE_2D, gBuffer3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width(), height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void MainView::loadModel(QString filename, GLuint bufferObject) {

    models.append(new Model(filename));
    models[models.size() - 1]->unitize();
    numTris = models[models.size() - 1]->getNumTriangles();

    Q_UNUSED(bufferObject);
}

void MainView::loadTexture(QString file) {
    QImageReader imgreader(file);
    QImage img = imgreader.read();
    textures.append(imageToBytes(img));
    texWidths.append(img.width());
    texHeights.append(img.height());
}

void MainView::updateBuffers(int index) {
    vertices = models[index]->getVertices();
    normals = models[index]->getNormals();
    vertexNumber = vertices.size();
    textureCoords = models[index]->getTextureCoords();

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(QVector3D), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,CBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(QVector3D), colors.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,NBO);
    glBufferData(GL_ARRAY_BUFFER, vertexNumber * sizeof(QVector3D), normals.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,TBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(QVector2D), textureCoords.data(), GL_DYNAMIC_DRAW);
}

void MainView::updateTextures(int index) {
    glBindTexture(GL_TEXTURE_2D, texPtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texWidths[index], texHeights[index], 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[index].data());
    glGenerateMipmap(GL_TEXTURE_2D);
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

    glGenTextures(1,&texPtr);
    loadTexture(":/textures/cat_diff.png");
    loadTexture(":/textures/cat_norm.png");
    loadTexture(":/textures/cat_spec.png");
    loadModel(":/models/cat.obj", cubeBO);

    createShaderPrograms();

    createTextures();
    createBuffers();

    shaderprog2->bind();
    glUniform1i(shader2Diffuse, 0);
    glUniform1i(shader2Normal, 1);
    glUniform1i(shader2Depth, 2);
    shaderprog2->release();

    eye = QVector3D {0,0,10};
    viewDirection = - eye;
    updateScale(currentScale);

    initializeScene();

    // For animation, you can start your timer here
    timer.start(1000/60);
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
    glBindTexture(GL_TEXTURE_2D, gBuffer1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, gBuffer2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, gBuffer3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width(), height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&defaultFramebuffer);

    updateCameraPosition();
    updateMatrices();

    mainShaderProg->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPtr);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glBindVertexArray(VAO);

    // Clear the screen before rendering
    glClearColor(0.0f,0.0f,0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderScene();

    glBindVertexArray(0);
    mainShaderProg->release();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFramebuffer);

    QVector<QVector2D> trios {
        QVector2D (-1,-1),
        QVector2D (-1,1),
        QVector2D (1,1),
        QVector2D (-1,-1),
        QVector2D (1,-1),
        QVector2D (1,1)
    };
    QVector<QVector2D> uvs {
        QVector2D (0,0),
        QVector2D (0,1),
        QVector2D (1,1),
        QVector2D (0,0),
        QVector2D (1,0),
        QVector2D (1,1)
    };

    shaderprog2->bind();
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(QVector2D), trios.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,UVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(QVector2D), uvs.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer3);
    glActiveTexture(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shaderprog2->release();
}

void MainView::updateMatrices() {
    view.setToIdentity();
    projection.setToIdentity();

    if (viewMode == 0) {
        view.lookAt(eye, QVector3D {0,0,0}, QVector3D {0, 1, 0});
    } else {
        view.lookAt(eye, eye + viewDirection, QVector3D {0, 1, 0});
    }
    projection.perspective(30.0, (float)width() / (float)height(), 0.01, 11000.0);
    model = rotation * scaling;
    normal = rotation;

    mainShaderProg->bind();
    glUniformMatrix4fv(shaderModel, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(shaderView, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(shaderProjection, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(shaderNormal, 1, GL_FALSE, normal.data());
    mainShaderProg->release();

    shaderprog2->bind();
    glUniform4f(shader2Components, 0.2, 0.6, 0.2, 3);
    glUniform3f(shader2EyePos, eye[0], eye[1], eye[2]);
    QMatrix4x4 inv = projection * view * model;
    inv = inv.inverted();
    glUniformMatrix4fv(shader2Inverse, 1, GL_FALSE, inv.data());
    shaderprog2->release();
}
