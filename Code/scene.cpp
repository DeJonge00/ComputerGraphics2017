#include "mainview.h"

void MainView::renderTexturedModel(int modelIndex, int texIndex, QVector3D pos, float size, QVector4D phong)
{
    updateBuffers(modelIndex);
    updateTextures(texIndex);
    glUniform3f(shaderPosition, pos[0], pos[1], pos[2]);
    glUniform1f(shaderSize,size);
    glUniform3f(shaderLightPos, lightPos[0], lightPos[1], lightPos[2]);
    glUniform3f(shaderLightColor, lightCol[0], lightCol[1], lightCol[2]);
    glUniform4f(shaderComponents, phong[0], phong[1], phong[2], phong[3]);
    glDrawArrays(GL_TRIANGLES, 0, vertexNumber);
}

void MainView::renderColoredModel(int modelIndex, QVector3D matColor, QVector3D pos, float size, QVector4D phong)
{
    //TODO: remove texture
    updateBuffers(modelIndex);
    glUniform3f(shaderMatColor, matColor[0], matColor[1], matColor[2]);
    glUniform3f(shaderPosition, pos[0], pos[1], pos[2]);
    glUniform1f(shaderSize,size);
    glUniform3f(shaderLightPos, lightPos[0], lightPos[1], lightPos[2]);
    glUniform3f(shaderLightColor, lightCol[0], lightCol[1], lightCol[2]);
    glUniform4f(shaderComponents, phong[0], phong[1], phong[2], phong[3]);
    glDrawArrays(GL_TRIANGLES, 0, vertexNumber);
}

/**
 * Renders a scene, containing textured and colored models.
 */
void MainView::renderScene()
{
    lightPos = QVector3D(-20,0,0);
    lightCol = QVector3D(1,1,1);

    renderTexturedModel(0, 0, QVector3D(0,0,0), 1, QVector4D(0.6,0.2,0.2,1.0));
    renderTexturedModel(1, 1, QVector3D(4,0,0), 2, QVector4D(0.6,0.2,0.2,1.0));
}
