#include "mainview.h"

void MainView::renderModel(SceneObject a)
{
    QVector3D pos = a.getPosition();
    QVector2D rot = a.getLocalRotation();
    updateBuffers(a.modelIndex);
    updateTextures(a.texIndex);
    glUniform1f(shaderSize,a.getSize());
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

    for(int i = 0; i < sceneobjects.size(); i++) {
        renderModel(*sceneobjects[i]);
    }
}

void MainView::initializeScene() {
    shaderprog2->bind();
    glUniform3f(shader2LightPos, 100, 0, 0);
    glUniform3f(shader2LightColor, 1, 1, 1);
    SceneObject* obj;

    obj = new SceneObject(0, 0, QVector3D(0,0,0));
    obj->setSize(100);
    sceneobjects.append(obj);
    shaderprog2->release();
}
