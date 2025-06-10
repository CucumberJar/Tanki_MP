#include "Wall.h"
#include <QPixmap>

#include <QPainter>


QRectF Wall::boundingRect() const {
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QGraphicsPixmapItem::paint(painter, option, widget);
}

void Wall::setImage(QString way ){
    QPixmap boxPixmap(way);
    setPixmap(boxPixmap);
}
Wall::Wall(qreal x, qreal y) {
    setPos(x*32, y*32);
    setImage("../drawable/texture/wall.png");
}
