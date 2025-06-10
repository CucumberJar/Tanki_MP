#include "Bullet.h"
#include <QtMath>
#include <QPainter>
#include <QGraphicsScene>
#include <QWidget>
#include "Wall.h"
#include "../src/map/Stone.h"
#include "../src/tank/entities/Tank.h"
Bullet::Bullet(qreal angle)
        : angle(angle), speed(10), radius(4)  // радиус шара
{
    setRotation(angle);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);
    timer->start(10); // ~60 FPS
    setFlag(QGraphicsItem::ItemIsFocusable, false); // Пуля НЕ фокусируется

}

QRectF Bullet::boundingRect() const
{
    return QRectF(-radius, -radius, 2 * radius, 2 * radius);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    setZValue(-1);
    painter->setBrush(Qt::red);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(boundingRect());
}

void Bullet::move()
{
    setZValue(-1);
    qreal rad = qDegreesToRadians(angle);
    QPointF delta(speed * std::sin(rad), -speed * std::cos(rad));
    setPos(pos() + delta);
    checkCollision();
}


bool Bullet::checkCollision() {
    for (QGraphicsItem *item : scene()->collidingItems(this)) {
        if (item == this) continue;  // Пропускаем саму пулю

        if (Stone *stone = dynamic_cast<Stone *>(item)) {
            QString msg=QString("BLOCK_DESTROYED;%1;%2\n").arg(int(stone->x())).arg(int(stone->y()));
            client->getSocket()->write(msg.toUtf8());
            client->getSocket()->flush();
            deleteLater();
            return true;
        }
        if (dynamic_cast<Wall *>(item)) {
            scene()->removeItem(this);
            deleteLater();
            return true;
        }
        if (Tank *tank = dynamic_cast<Tank *>(item)) {
            if (tank->isLocal()) return true;

            tank->setHp(tank->getHp() - 20);
            deleteLater();
            if (tank->getHp() <= 0) {
                tank->destroy();
                QString msg=QString("TANK_DESTROYED;%1\n").arg(tank->getPlayerId());
                client->getSocket()->write(msg.toUtf8());
                client->getSocket()->flush();

            }
            return true;
        }

    }
    return false;
}

const QString &Bullet::getPlayerId() const {
    return playerId;
}

void Bullet::setPlayerId(const QString &playerId) {
    Bullet::playerId = playerId;
}
