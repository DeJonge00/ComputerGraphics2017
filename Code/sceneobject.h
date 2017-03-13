#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QVector3D>
#include <QVector2D>
#include <QDebug>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include "cmath"

#define pi 3.141592653589793238

class SceneObject {
public:
    SceneObject(int model, int texture, QVector3D pos);
    ~SceneObject();

    void updateTime();

    QVector3D getPosition();
    float getSize();
    QVector2D getLocalRotation();

    void setLocalRotation(float speed);
    void setGlobalRotation(float speed, QVector3D tilt, QVector2D rad);
    void setGlobalRotation(float speed, QVector3D tilt, float rad);
    void setRotationCenterObject(SceneObject *obj);
    void setSize(float s);

    int modelIndex, texIndex;
    bool lightingOn = true;
    QVector4D phong;
private:
    QVector3D globalTilt, rotationCenter;
    QVector2D radii;
    float localSpeed, globalSpeed, currentGlobalRotation, currentLocalRotation, size;
    SceneObject *centerObject;

};

#endif // SCENEOBJECT_H
