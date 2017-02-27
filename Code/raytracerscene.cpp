#include "mainview.h"

void MainView::renderSphere(QVector3D pos, QVector3D color, QVector4D material, QVector3D lightpos)
{
    glUniform3f(shaderMatColor, color[0], color[1], color[2]);
    glUniform3f(shaderPosition, pos[0], pos[1], pos[2]);
    glUniform3f(shaderLightPos, lightpos[0], lightpos[1], lightpos[2]);
    glUniform3f(shaderLightColor, 1.0f, 1.0f, 1.0f);
    glUniform4f(shaderComponents, material[0], material[1], material[2], material[3]);
    glDrawArrays(GL_TRIANGLES, 0, vertexNumber);
}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainView::renderRaytracerScene()
{
    QVector3D lightpos = QVector3D(-200,600,1500);

    // Blue sphere
    renderSphere(QVector3D(90,320,100),QVector3D(0,0,1),QVector4D(0.2f,0.7f,0.5f,64),lightpos);

    // Green sphere
    renderSphere(QVector3D(210,270,300),QVector3D(0,1,0),QVector4D(0.2f,0.3f,0.5f,8),lightpos);

    // Red sphere
    renderSphere(QVector3D(290,170,150),QVector3D(1,0,0),QVector4D(0.2f,0.7f,0.8f,32),lightpos);

    // Yellow sphere
    renderSphere(QVector3D(140,220,400),QVector3D(1,0.8f,0),QVector4D(0.2f,0.8f,0.0f,1),lightpos);

    // Orange sphere
    renderSphere(QVector3D(110,130,200),QVector3D(1,0.5f,0),QVector4D(0.2f,0.8f,0.5f,32),lightpos);
}
