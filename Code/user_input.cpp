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
    switch(ev->key()) {
    case 'A': qDebug() << "A pressed"; break;
    default:
        // ev->key() is an integer. For alpha numeric characters keys it equivalent with the char value ('A' == 65, '1' == 49)
        // Alternatively, you could use Qt Key enums, see http://doc.qt.io/qt-5/qt.html#Key-enum
        qDebug() << ev->key() << "pressed";
        break;
    }

    // Used to update the screen after changes
    update();
}

// Triggered by releasing a key
void MainView::keyReleaseEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'A': qDebug() << "A released"; break;
    default:
        qDebug() << ev->key() << "released";
        break;
    }

    update();
}

// Triggered by clicking two subsequent times on any mouse button
// It also fires two mousePress and mouseRelease events!
void MainView::mouseDoubleClickEvent(QMouseEvent *ev)
{

}

// Triggered when moving the mouse inside the window (only when the mouse is clicked!)
void MainView::mouseMoveEvent(QMouseEvent *ev)
{
    currentRotation.setX(ev->x() - prevMouseX + currentRotation.x());
    currentRotation.setY(ev->y() - prevMouseY + currentRotation.y());
    prevMouseX = ev->x();
    prevMouseY = ev->y();
    updateRotation(-currentRotation.y(),currentRotation.x(),0);
}

// Triggered when pressing any mouse button
void MainView::mousePressEvent(QMouseEvent *ev)
{
    prevMouseX = ev->x();
    prevMouseY = ev->y();
    // Do not remove the line below, clicking must focus on this widget!
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
