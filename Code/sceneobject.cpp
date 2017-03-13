#include "sceneobject.h"

SceneObject::SceneObject(int model, int texture, QVector3D pos) {
    modelIndex = model;
    texIndex = texture;
    rotationCenter = pos;
    centerObject = this;
    currentLocalRotation = 0;
    currentGlobalRotation = 0;
    globalSpeed = 0;
    localSpeed = 0;
    size = 1;
    phong = QVector4D {0.3,0.6,0.1,10};
}

SceneObject::~SceneObject() {

}

/* Calculate the new rotations for the next timeframe */
void SceneObject::updateTime() {
    currentGlobalRotation += globalSpeed;
    currentLocalRotation += localSpeed;
    if (centerObject != this) {
        rotationCenter = centerObject->getPosition();
    }
}

/* Calculate the position of the object in the current timeframe */
QVector3D SceneObject::getPosition() {
    QVector3D currentPosition = rotationCenter;
    QVector3D rotpos {
        cosf(currentGlobalRotation) * radii[0],
        0,
        sinf(currentGlobalRotation) * radii[1]
    };
    float xrad = globalTilt.x();
    float yrad = globalTilt.y();
    float zrad = globalTilt.z();
    QMatrix4x4 rotX {
        1, 0, 0, 0,
        0, cosf(xrad), -sinf(xrad), 0,
        0, sinf(xrad), cosf(xrad), 0,
        0, 0, 0, 1
    };
    QMatrix4x4 rotY {
        cosf(yrad), 0, sinf(yrad), 0,
        0, 1, 0, 0,
        -sinf(yrad), 0, cosf(yrad), 0,
        0, 0, 0, 1
    };
    QMatrix4x4 rotZ {
        cosf(zrad), -sinf(zrad), 0, 0,
        sinf(zrad), cosf(zrad), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    rotpos = QVector3D(QVector4D(rotpos) * rotX * rotY * rotZ);
    return currentPosition + rotpos;
}

/* get the size of the object */
float SceneObject::getSize() {
    return size;
}

/* Get the current rotation of the object around its axis */
QVector2D SceneObject::getLocalRotation() {
    return QVector2D {currentLocalRotation, 0};
}

/* Make the object rotate around its axis with the specified speed */
void SceneObject::setLocalRotation(float speed) {
    localSpeed = speed * pi / 3600;
}

/* Makes the object rotate around a point or another object with the specified speed and radius */
void SceneObject::setGlobalRotation(float speed, QVector3D tilt, float rad) {
    globalSpeed = speed * pi / 900;
    globalTilt = tilt;
    radii = QVector2D(rad, rad);
}

/* Makes the object rotate around a point or another object with the specified speed,
   But alows for two radii to be specified, causing an elliptical orbit */
void SceneObject::setGlobalRotation(float speed, QVector3D tilt, QVector2D rad) {
    globalSpeed = speed * pi / 900;
    globalTilt = tilt;
    radii = rad;
}

/* Set the object around which this object is rotating */
void SceneObject::setRotationCenterObject(SceneObject *obj) {
    centerObject = obj;
}

/* Set the size of the object */
void SceneObject::setSize(float s) {
    size = s;
}
