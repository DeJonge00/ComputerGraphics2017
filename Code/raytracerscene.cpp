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

void MainView::renderCube(QVector3D pos, QVector3D lightpos)
{
    glUniform3f(shaderMatColor, 1, 1, 1);
    glUniform3f(shaderPosition, pos[0], pos[1], pos[2]);
    glUniform3f(shaderLightPos, lightpos[0], lightpos[1], lightpos[2]);
    glUniform3f(shaderLightColor, 1.0f, 1.0f, 1.0f);
    glUniform4f(shaderComponents, 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, vertexNumber);
}

/**
 * Renders a similar scene used for the raytracer:
 * 5 colored spheres with a single light
 */
void MainView::renderRaytracerScene()
{
    QVector3D lightpos = QVector3D(-20,0,0);

    //Textured Cube
    renderCube(QVector3D(0,0,0),lightpos);
}
