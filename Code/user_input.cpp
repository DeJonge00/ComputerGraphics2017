#include "mainview.h"

#include <QDebug>

void MainView::updateRotation(int x, int y, int z)
{
    float xrad = x*pi/180;
    float yrad = y*pi/180;
    float zrad = z*pi/180;
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
    rotation = rotX * rotY * rotZ;
}

void MainView::updateModel(QString name)
{
    qDebug() << "updateModel(" << name << ")";
    // TODO: load new model, based on name
}

void MainView::updateShader(QString name)
{
    qDebug() << "updateShader(" << name << ")";
    // TODO: switch shaders/renderingprocesses, based on name
}

void MainView::updateScale(float scale)
{
    scaling = QMatrix4x4 {
        scale, 0, 0, 0,
        0, scale, 0, 0,
        0, 0, scale, 0,
        0, 0, 0, 1
    };
}


// Triggered by pressing a key
void MainView::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == 32) {
        //viewMode = 1 - viewMode;
    }

    if (viewMode != 1) {
        switch(ev->key()) {
            case 'W': centerIndex++; break;
            case 'S': centerIndex--; break;
        }
        if (centerIndex < 0) {
            centerIndex = sceneobjects.size() - 1;
        } else if (centerIndex >= sceneobjects.size()) {
            centerIndex = 0;
        }
        return;
    }
    switch(ev->key()) {
        case 'W': forpressed = true; break;
        case 'A': leftpressed = true; break;
        case 'S': backpressed = true; break;
        case 'D': rightpressed = true; break;
        case 'E': uppressed = true; break;
        case 'Q': downpressed = true; break;
    }
}

// Triggered by releasing a key
void MainView::keyReleaseEvent(QKeyEvent *ev)
{
    if (viewMode != 1) {
        return;
    }
    switch(ev->key()) {
        case 'W': forpressed = false; break;
        case 'A': leftpressed = false; break;
        case 'S': backpressed = false; break;
        case 'D': rightpressed = false; break;
        case 'E': uppressed = false; break;
        case 'Q': downpressed = false; break;
    }
}

void MainView::updateCameraPosition() {
    QVector3D movement {0, 0, 0};
    QVector3D noMovement {0, 0, 0};
    QVector3D right {
        -viewDirection[2],
        viewDirection[1],
        viewDirection[0]
    };
    QVector3D up {
        viewDirection[0],
        -viewDirection[2],
        viewDirection[1]
    };
    movement += (rightpressed ? right : noMovement) - (leftpressed ? right : noMovement);
    movement += (uppressed ? up : noMovement) - (downpressed ? up : noMovement);
    movement += (forpressed ? viewDirection : noMovement) - (backpressed ? viewDirection : noMovement);

    eye += moveSpeed * movement;
}

// Triggered by clicking two subsequent times on any mouse button
// It also fires two mousePress and mouseRelease events!
void MainView::mouseDoubleClickEvent(QMouseEvent *ev)
{

}

// Triggered when moving the mouse inside the window (only when the mouse is clicked!)
void MainView::mouseMoveEvent(QMouseEvent *ev)
{
    currentRotation[0] += ev->x() - prevMouseX;
    currentRotation[1] += ev->y() - prevMouseY;
    if (viewMode == 0) {
        updateRotation(currentRotation.y(),currentRotation.x(),0);
    } else {
        viewDirection = QVector3D {
            cosf(-currentRotation[0] / 1440 * pi - pi/2) * cosf(currentRotation[1] / 1440 * pi + pi),
            sinf(currentRotation[1] / 1440 * pi + pi),
            sinf(-currentRotation[0] / 1440 * pi - pi/2)
        };
        viewDirection = viewDirection.normalized();
    }
    prevMouseX = ev->x();
    prevMouseY = ev->y();
}

// Triggered when pressing any mouse button
void MainView::mousePressEvent(QMouseEvent *ev)
{
    prevMouseX = ev->x();
    prevMouseY = ev->y();
    this->setFocus();
}

// Triggered when releasing any mouse button
void MainView::mouseReleaseEvent(QMouseEvent *ev)
{

}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainView::wheelEvent(QWheelEvent *ev)
{
    if (ev->angleDelta().y() > 0) {
        currentScale *= ev->angleDelta().y();
        currentScale /= 100;
    } else if (currentScale > 0.007) {
        currentScale /= -ev->angleDelta().y();
        currentScale *= 100;
    }
    if (currentScale < 0.007) {
        currentScale = 0.007;
    }
    updateScale(currentScale);
}
