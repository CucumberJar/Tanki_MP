#include "Tank.h"
#include <QKeyEvent>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

Tank::Tank() {
    setRect(0, 0, 64, 128);
    setTransformOriginPoint(32, 64);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    qDebug() << "Tank created!";

    // Создаём и позиционируем башню
    turret = new Turret(this);
    turret->setPos(12, 32); // Смещение башни в центр верхней части

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Tank::updateMovement);
    moveTimer->start(16);
}

void Tank::rotateTurret(const QPointF &cursorPosition) {
    if (!turret) return;

    // Передаём координаты курсора в башню, чтобы она сама обрабатывала поворот плавно
    turret->rotateToCursor(cursorPosition);
}



void Tank::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "Mouse moved at:" << event->scenePos();
    turret->rotateToCursor(event->scenePos()); // Поворачиваем башню к курсору
}




void Tank::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            isMoving = true;
            break;
        case Qt::Key_A:
            isRotatingLeft = true;
            break;
        case Qt::Key_D:
            isRotatingRight = true;
            break;
        default:
            break;
    }
}

void Tank::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            isMoving = false;
            break;
        case Qt::Key_A:
            isRotatingLeft = false;
            break;
        case Qt::Key_D:
            isRotatingRight = false;
            break;
        default:
            break;
    }
}





// Двигаем и поворачиваем танк одновременно
void Tank::updateMovement() {
    if (isRotatingLeft) {
        setRotation(rotation() - rotationSpeed);
    }
    if (isRotatingRight) {
        setRotation(rotation() + rotationSpeed);
    }
    if (isMoving) {
        qreal angle = rotation();
        qreal radians = qDegreesToRadians(angle);
        qreal dx = speed * qSin(radians);
        qreal dy = -speed * qCos(radians);
        setPos(x() + dx, y() + dy);
    }
}
