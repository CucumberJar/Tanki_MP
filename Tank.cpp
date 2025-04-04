#include "Tank.h"
#include "Box.h"
#include <QGraphicsView>

#include <QCursor>
#include "Turret.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QTransform>
#include <QTimer>
#include <cmath>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QLineF>

Tank::Tank(QGraphicsScene *scene)
        : speed(4), angle(0),
          movingUp(false), movingDown(false),
          rotatingLeft(false), rotatingRight(false)
{
    QPixmap originalPixmap("D:/untitled5/drawable/tank.png");
    setPixmap(originalPixmap);
    setTransformOriginPoint(pixmap().width() / 2, pixmap().height() / 2);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Tank::move);
    timer->start(16);
    scene->addItem(this);
    turret = new Turret(this);
    turret->setZValue(1);

// Центрируем башню по центру танка
    turret->setPos(-15,14);

    setAcceptHoverEvents(true);

    setPos(100, 100);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}


void Tank::updateTurretRotation() {
    if (!turret) return;

    // Центр башни в сцене
    QPointF turretCenter = turret->mapToScene(turret->boundingRect().center());

    // Позиция курсора в сцене
    QPoint cursorPos = QCursor::pos();
    QPointF cursorScenePos = scene()->views().first()->mapToScene(cursorPos);

    // Направление от центра башни до курсора
    QLineF lineToCursor(turretCenter, cursorScenePos);
    qreal targetAngle = -lineToCursor.angle(); // Негативный угол для корректного вращения по часовой стрелке

    // Вычисляем угол для башни с учетом угла танка (когда танк поворачивается, башня также должна вращаться)
    qreal turretAngle = targetAngle - angle;

    // Ограничиваем скорость поворота башни
    const qreal MAX_TURRET_ROTATION_SPEED = 5.0; // Максимальная скорость поворота в градусах
    qreal deltaAngle = turretAngle - turret->rotation();
    if (deltaAngle > 180) {
        deltaAngle -= 360;
    } else if (deltaAngle < -180) {
        deltaAngle += 360;
    }

    if (deltaAngle > MAX_TURRET_ROTATION_SPEED) {
        deltaAngle = MAX_TURRET_ROTATION_SPEED;
    } else if (deltaAngle < -MAX_TURRET_ROTATION_SPEED) {
        deltaAngle = -MAX_TURRET_ROTATION_SPEED;
    }

    // Поворачиваем башню
    turret->setRotation(turret->rotation() + deltaAngle);
}



void Tank::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W: movingUp = true; break;
        case Qt::Key_S: movingDown = true; break;
        case Qt::Key_A: rotatingLeft = true; break;
        case Qt::Key_D: rotatingRight = true; break;
    }
}

void Tank::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W: movingUp = false; break;
        case Qt::Key_S: movingDown = false; break;
        case Qt::Key_A: rotatingLeft = false; break;
        case Qt::Key_D: rotatingRight = false; break;
    }
}

QRectF Tank::boundingRect() const {
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
    // painter->setPen(QPen(Qt::red, 1));
    // painter->drawRect(boundingRect());  // можно включить для отладки
}

void Tank::move() {

    updateTurretRotation();
    qreal newAngle = angle;
    if (rotatingLeft)  newAngle -= 1;
    if (rotatingRight) newAngle += 1;

    if (newAngle != angle) {
        angle = newAngle;
        setRotation(angle);
        if (checkCollision()) {
            angle = newAngle - (rotatingLeft ? -1 : 1);
            setRotation(angle);
        }
    }

    if (!movingUp && !movingDown) return;
    qreal rad = qDegreesToRadians(angle);
    QPointF delta;
    if (movingUp) {
        delta = QPointF(speed * std::sin(rad), -speed * std::cos(rad));
    } else if (movingDown) {
        delta = -QPointF(speed * std::sin(rad), -speed * std::cos(rad));
    }
    QPointF newPos = pos() + delta;
    setPos(newPos);
    if (checkCollision()) {
        setPos(pos() - delta);
    }
    ;  // следим за курсором каждый кадр

}

bool Tank::checkCollision() {
    for (QGraphicsItem *item : scene()->collidingItems(this)) {
        if (item == this) continue;
        if (dynamic_cast<Box*>(item)) {
            return true;
        }
    }
    return false;
}
