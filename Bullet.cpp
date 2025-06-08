#include "Bullet.h"
#include <QtMath>
#include <QPainter>
#include <QGraphicsScene>
#include <QWidget>
#include "Wall.h"
#include "../src/map/Stone.h"
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
    setZValue(-2);
    painter->setBrush(Qt::red);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(boundingRect());
}

void Bullet::move()
{
    setZValue(-2);
    qreal rad = qDegreesToRadians(angle);
    QPointF delta(speed * std::sin(rad), -speed * std::cos(rad));
    setPos(pos() + delta);
    checkCollision();
}


bool Bullet::checkCollision() {
    // Получаем все объекты, пересекающиеся с пулей
    for (QGraphicsItem *item : scene()->collidingItems(this)) {
        if (item == this) continue;  // Пропускаем саму пулю

      /*  if (Tank *tank = dynamic_cast<Tank *>(item)) {
            // Можно добавить проверку команды танка, чтобы не попадать в свой
            tank->takeDamage(10);  // например, 10 урона
            return true;  // коллизия с танком
        }*/

        if (Wall *wall = dynamic_cast<Wall *>(item)) {
            deleteLater();
            return true;
        }

        if (Stone *stone = dynamic_cast<Stone *>(item)) {
            scene()->removeItem(stone);
            deleteLater();
            return true;
        }


      /*  if (Base *base = dynamic_cast<Base *>(item)) {
            base->takeDamage(10);  // урон базе
            return true;
        }*/
    }
    return false;
}
