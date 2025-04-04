#include "Tank.h"
#include "Box.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QTransform>
#include <QTimer>
#include <cmath>
#include <QPainter>

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
    setPos(100, 100);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
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
