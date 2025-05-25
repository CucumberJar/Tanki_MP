#include "Tank.h"
#include "../../../Wall.h"
#include "../../../Turret.h"
#include "../../../Bullet.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QCursor>
#include <QTimer>
#include <QPainter>
#include <QLineF>
#include <cmath>

Tank::Tank(bool isLocal, const QString &id, TankClient *clientPtr)
        : QObject(), QGraphicsPixmapItem(), isLocalTank(isLocal), playerId(id), client(clientPtr) {

    QPixmap originalPixmap("../drawable/tank.png");
    setPixmap(originalPixmap);
    setTransformOriginPoint(pixmap().width() / 2, pixmap().height() / 2);

    turret = new Turret(this);
    turret->setZValue(1);
    turret->setRotation(0);
    turret->setPos(pixmap().width() / 2 - 32, pixmap().height() / 2 - 90);

    setFlag(QGraphicsItem::ItemIsFocusable);
    if (isLocalTank) {
        setFocus();
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Tank::move);
    timer->start(16);
}

void Tank::updateTurretRotation() {
    if (!turret) return;
    QPointF turretCenter = turret->mapToScene(turret->boundingRect().center());
    QGraphicsView *view = scene()->views().first();
    QPoint cursorPos = QCursor::pos();
    QPoint cursorPosInView = view->viewport()->mapFromGlobal(cursorPos);
    QPointF cursorScenePos = view->mapToScene(cursorPosInView);
    QPointF direction = cursorScenePos - turretCenter;
    qreal targetAngle = qRadiansToDegrees(std::atan2(direction.y(), direction.x()));
    qreal turretRelativeAngle = targetAngle - angle + 90;
    while (turretRelativeAngle < -180) turretRelativeAngle += 360;
    while (turretRelativeAngle > 180) turretRelativeAngle -= 360;
    qreal deltaAngle = turretRelativeAngle - turret->rotation();
    while (deltaAngle < -180) deltaAngle += 360;
    while (deltaAngle > 180) deltaAngle -= 360;
    const qreal MAX_TURRET_ROTATION_SPEED = 0.5;
    if (deltaAngle > MAX_TURRET_ROTATION_SPEED) deltaAngle = MAX_TURRET_ROTATION_SPEED;
    if (deltaAngle < -MAX_TURRET_ROTATION_SPEED) deltaAngle = -MAX_TURRET_ROTATION_SPEED;
    turret->setRotation(turret->rotation() + deltaAngle);
}

void Tank::keyPressEvent(QKeyEvent *event) {
    if (!isLocalTank) return;
    switch (event->key()) {
        case Qt::Key_W: movingUp = true; break;
        case Qt::Key_S: movingDown = true; break;
        case Qt::Key_A: rotatingLeft = true; break;
        case Qt::Key_D: rotatingRight = true; break;
    }
}

void Tank::keyReleaseEvent(QKeyEvent *event) {
    if (!isLocalTank) return;
    switch (event->key()) {
        case Qt::Key_W: movingUp = false; break;
        case Qt::Key_S: movingDown = false; break;
        case Qt::Key_A: rotatingLeft = false; break;
        case Qt::Key_D: rotatingRight = false; break;
    }
}

void Tank::move() {
    if (!isLocalTank) return;
    if (!hasFocus()) setFocus();
    updateTurretRotation();

    qreal newAngle = angle;
    if (rotatingLeft) newAngle -= 1;
    if (rotatingRight) newAngle += 1;

    if (newAngle != angle) {
        angle = newAngle;
        setRotation(angle);
        if (checkCollision()) {
            angle = newAngle - (rotatingLeft ? -1 : 1);
            setRotation(angle);
        }
    }

    QPointF delta;
    if (movingUp || movingDown) {
        qreal rad = qDegreesToRadians(angle);
        delta = QPointF(speed * std::sin(rad), -speed * std::cos(rad));
        if (movingDown) delta = -delta;
        QPointF newPos = pos() + delta;
        setPos(newPos);
        if (checkCollision()) setPos(pos() - delta);
    }

    if (isLocalTank && client) {
        QString msg = QString("MOVE;%1;%2;%3;%4\n")
                .arg(playerId)
                .arg(pos().x())
                .arg(pos().y())
                .arg(angle);
        client->getSocket()->write(msg.toUtf8());
        client->getSocket()->flush();
    }
}

QRectF Tank::boundingRect() const {
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
}

void Tank::shoot() {
    Bullet *bullet = new Bullet(turret->rotation() + angle);
    bullet->setZValue(-1);
    QPointF barrelTip = turret->mapToScene(QPointF(turret->boundingRect().width() / 2, 0));
    bullet->setPos(barrelTip);
    scene()->addItem(bullet);
}

bool Tank::checkCollision() {
    for (QGraphicsItem *item : scene()->collidingItems(this)) {
        if (item == this) continue;
        if (dynamic_cast<Wall *>(item)) return true;
    }
    return false;
}
