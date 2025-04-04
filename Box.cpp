#include "Box.h"
#include <QPixmap>

#include <QPainter>

QRectF Box::boundingRect() const {
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void Box::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);

    // Рисуем зелёный контур (хитбокс)
    painter->setPen(QPen(Qt::green, 2));
    painter->drawRect(boundingRect());
}

Box::Box(qreal x, qreal y) {
    QPixmap boxPixmap("D:/untitled5/drawable/box.png");
    setPixmap(boxPixmap);
    setPos(x, y);
}
