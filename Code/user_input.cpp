#include "mainview.h"

#include <QDebug>

void MainView::updateRotation(int x, int y, int z)
{
    double xrad = x*pi/180;
    double yrad = y*pi/180;
    double zrad = z*pi/180;
    QMatrix4x4 rotX {
        1, 0, 0, 0,
        0, cos(xrad), -sin(xrad), 0,
        0, sin(xrad), cos(xrad), 0,
        0, 0, 0, 1
    };
    QMatrix4x4 rotY {
        cos(yrad), 0, sin(yrad), 0,
        0, 1, 0, 0,
        -sin(yrad), 0, cos(yrad), 0,
        0, 0, 0, 1
    };
    QMatrix4x4 rotZ {
        cos(zrad), -sin(zrad), 0, 0,
        sin(zrad), cos(zrad), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    rotation = rotX * rotY * rotZ;
    update();
}

void MainView::updateModel(QString name)
{
    qDebug() << "updateModel(" << name << ")";
    // TODO: load new model, based on name

    update();
}

void MainView::updateShader(QString name)
{
    qDebug() << "updateShader(" << name << ")";
    // TODO: switch shaders/renderingprocesses, based on name

    update();
}

void MainView::updateScale(float scale)
{
    scaling = QMatrix4x4 {
        scale, 0, 0, 0,
        0, scale, 0, 0,
        0, 0, scale, 0,
        0, 0, 0, 1
    };
    update();
}


// Triggered by pressing a key
void MainView::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == 32) {
        viewMode = 1 - viewMode; //spacebar switches between viewmodes
    }

    if (viewMode != 1) {
        update();
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
    update();
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
    update();
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
    qDebug() << viewDirection;
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
            cos(-currentRotation[0] / 1440 * pi - pi/2) * cos(currentRotation[1] / 1440 * pi + pi),
            sin(currentRotation[1] / 1440 * pi + pi),
            sin(-currentRotation[0] / 1440 * pi - pi/2)
        };
        viewDirection = viewDirection.normalized();
    }
    prevMouseX = ev->x();
    prevMouseY = ev->y();
    update();
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
    if (ev->angleDelta().y() >= 0) {
        currentScale *= ev->angleDelta().y();
        currentScale /= 100;
    } else {
        currentScale /= -ev->angleDelta().y();
        currentScale *= 100;
    }
    qDebug() << currentScale << ev->angleDelta();
    updateScale(currentScale);
}
