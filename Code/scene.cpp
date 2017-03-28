#include "mainview.h"

void MainView::renderModel(SceneObject a)
{
    if (a.lightingOn) {
        glUniform1f(shaderLightingOn, 1.0);
    } else {
        glUniform1f(shaderLightingOn, 0.0);
    }
    QVector3D pos = a.getPosition();
    QVector2D rot = a.getLocalRotation();
    updateBuffers(a.modelIndex);
    updateTextures(a.texIndex);
    glUniform3f(shaderPosition, pos[0], pos[1], pos[2]);
    glUniform1f(shaderSize,a.getSize());
    glUniform4f(shaderComponents, a.phong[0], a.phong[1], a.phong[2], a.phong[3]);
    glUniform2f(shaderRotation, rot[0], rot[1]);
    glDrawArrays(GL_TRIANGLES, 0, vertexNumber);
}

/*
 * Renders a scene, containing textured and colored models.
 */
void MainView::renderScene()
{
    for(int i = 0; i < sceneobjects.size(); i++) {
        sceneobjects[i]->updateTime();
    }

    QVector3D centerpos = sceneobjects[centerIndex]->getPosition();
    glUniform3f(shaderCenterPos, centerpos[0], centerpos[1], centerpos[2]);

    for(int i = 0; i < sceneobjects.size(); i++) {
        renderModel(*sceneobjects[i]);
    }
}

void MainView::initializeScene() {
    mainShaderProg->bind();
    lightCol = QVector3D(1,1,1);
    lightPos = QVector3D(0,0,0);
    glUniform3f(shaderLightPos, lightPos[0], lightPos[1], lightPos[2]);
    glUniform3f(shaderLightColor, lightCol[0], lightCol[1], lightCol[2]);
    SceneObject* obj;
    centerIndex = 0;

    obj = new SceneObject(0, 0, QVector3D(0,0,0));
    obj->setSize(100);
    sceneobjects.append(obj);
    mainShaderProg->release();
}
