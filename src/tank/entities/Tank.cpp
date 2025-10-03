#include "Tank.h"
#include "map/Wall.h"
#include "Turret.h"
#include "Bullet.h"
#include "Base.h"
#include "Stone.h"
#include <QGraphicsView>
#include <QCursor>
#include <QTimer>
#include <QPainter>
#include <QLineF>
#include <cmath>

Tank::Tank(bool isLocal, const QString &id, TankClient *clientPtr)
        : QObject(), QGraphicsPixmapItem(), isLocalTank(isLocal), playerId(id), client(clientPtr) {
    QPixmap originalPixmap("../drawable/tank99.png");
    setPixmap(originalPixmap);
    setTransformOriginPoint(pixmap().width() / 2, pixmap().height() / 2);
    turret = new Turret(this);
    turret->setZValue(10);
    turret->setRotation(0);
    turret->setPos(pixmap().width() / 2 -32, pixmap().height() / 2 -90);
    setFlag(QGraphicsItem::ItemIsFocusable);
    if (isLocalTank) setFocus();
    lastSentTurretAngle = turret->rotation();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Tank::move);
    timer->start(20);
    shootCooldownTimer = new QTimer(this);
    shootCooldownTimer->setInterval(500);
    shootCooldownTimer->setSingleShot(true);
    connect(shootCooldownTimer, &QTimer::timeout, [this]() {
        canShoot = true;});
    canShoot = true;
}

void Tank::updateTurretRotation() {
    if (!turret || !isLocalTank) return;
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
    qreal posThreshold = 0.1;
    qreal angleThreshold = 2;
    if (client) {
        QPointF currentPos = pos();
        qreal currentTurretAngle = turret->rotation();
        bool posChanged = qAbs(currentPos.x() - lastSentPos.x()) > posThreshold ||
                          qAbs(currentPos.y() - lastSentPos.y()) > posThreshold;
        bool bodyAngleChanged = qAbs(angle - lastSentAngle) > angleThreshold;
        bool turretAngleChanged = qAbs(currentTurretAngle - lastSentTurretAngle) > angleThreshold;
        if (posChanged || bodyAngleChanged || turretAngleChanged) {
            QString msg = QString("MOVE;%1;%2;%3;%4;%5\n")
                    .arg(playerId)
                    .arg(currentPos.x(), 0, 'f', 2)
                    .arg(currentPos.y(), 0, 'f', 2)
                    .arg(angle, 0, 'f', 2)
                    .arg(currentTurretAngle, 0, 'f', 2);
            client->getSocket()->write(msg.toUtf8());
            client->getSocket()->flush();
            lastSentPos = currentPos;
            lastSentAngle = angle;
            lastSentTurretAngle = currentTurretAngle;
        }
    }
}

QRectF Tank::boundingRect() const {
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
}
void Tank::destroy() {
    isLocalTank= false;
    QPixmap originalPixmap("../drawable/tankD.png");
    setPixmap(originalPixmap);
    QPixmap originalPixmap1("../drawable/turretD.png");
    this->turret->setPixmap(originalPixmap1);
}
void Tank::shoot() {
    if (!canShoot) return;
    canShoot = false;
    shootCooldownTimer->start();
    Bullet *bullet = new Bullet(turret->rotation() + angle);
    bullet->setPlayerId(this->playerId);
    bullet->setClient(this->client);
    QPointF turretCenter = turret->mapToScene(turret->boundingRect().center());
    bullet->setPos(turretCenter);
    scene()->addItem(bullet);
}

bool Tank::checkCollision() {
    if (!scene()) return false;

    for (QGraphicsItem *item : scene()->collidingItems(this)) {
        if (item == this) continue;
        if (dynamic_cast<Tank *>(item)) return true;
        if (dynamic_cast<Wall *>(item)) return true;
        if (dynamic_cast<Stone *>(item)) return true;
        if (Base *base = dynamic_cast<Base *>(item)) {
            int newTeam = 0;
            if (base->getTeam() != newTeam) {
                this->speed += 0.2;
                int oldTeam = base->getTeam();
                base->setTeam(newTeam);
                if (client) {
                    QString msg = QString("CAPTURE;%1;%2\n").arg(oldTeam).arg(newTeam);
                }
            }
        }
    }
    return false;
}
