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
    centerIndex = 3;

    //sun
    obj = new SceneObject(0, 0, QVector3D(0,0,0));
    obj->setSize(108.9);
    obj->phong = QVector4D {1,0,0,0};
    sceneobjects.append(obj);

    //mercury
    obj = new SceneObject(0, 1, QVector3D(0,0,0));
    obj->setSize(0.38);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(4.14,QVector3D{0, 0, pi}, 120);
    obj->setLocalRotation(6.29/30);
    sceneobjects.append(obj);

    //venus
    obj = new SceneObject(0, 2, QVector3D(0,0,0));
    obj->setSize(0.95);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(1.62,QVector3D{0, 0, pi}, 130);
    obj->setLocalRotation(-3.14/30);
    sceneobjects.append(obj);

    //earth
    obj = new SceneObject(0, 3, QVector3D(0,0,0));
    obj->setSize(1.0);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(1,QVector3D{0, 0, pi}, 140);
    obj->setLocalRotation(365/30);
    sceneobjects.append(obj);

    //mars
    obj = new SceneObject(0, 4, QVector3D(0,0,0));
    obj->setSize(0.53);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(0.53,QVector3D{0, 0, pi}, 150);
    obj->setLocalRotation(365/30);
    sceneobjects.append(obj);

    //jupiter
    obj = new SceneObject(0, 5, QVector3D(0,0,0));
    obj->setSize(11.2);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(0.08,QVector3D{0, 0, pi}, 170);
    obj->setLocalRotation(973/30);
    sceneobjects.append(obj);

    //saturn
    obj = new SceneObject(0, 6, QVector3D(0,0,0));
    obj->setSize(9.4);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(0.0345,QVector3D{0, 0, pi}, 190);
    obj->setLocalRotation(876/30);
    sceneobjects.append(obj);

    //uranus
    obj = new SceneObject(0, 7, QVector3D(0,0,0));
    obj->setSize(4);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(0.0119,QVector3D{0, 0, pi}, 210);
    obj->setLocalRotation(547/30);
    sceneobjects.append(obj);

    //neptune
    obj = new SceneObject(0, 8, QVector3D(0,0,0));
    obj->setSize(3.88);
    obj->setRotationCenterObject(sceneobjects[0]);
    obj->setGlobalRotation(0.00606,QVector3D{0, 0, pi}, 230);
    obj->setLocalRotation(515/30);
    sceneobjects.append(obj);

    //moon
    obj = new SceneObject(0, 9, QVector3D(0,0,0));
    obj->setSize(0.3);
    obj->setRotationCenterObject(sceneobjects[3]);
    obj->setGlobalRotation(13.5,QVector3D{0, 0, 0}, 2);
    obj->setLocalRotation(13.5/30);
    sceneobjects.append(obj);

    //Milky way
    obj = new SceneObject(0, 10, QVector3D(0,0,0));
    obj->setSize(10000);
    obj->lightingOn = false;
    sceneobjects.append(obj);
    mainShaderProg->release();
}
