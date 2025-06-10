#include "Turret.h"
#include <QPixmap>
#include <QTransform>

Turret::Turret(QGraphicsItem *parent)
        : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap turretPixmap("D:/untitled5/drawable/turret0.png");
    setPixmap(turretPixmap);
    setZValue(10);
    setTransformOriginPoint(turretPixmap.width()/2, turretPixmap.height()/2);
}

void Turret::rotateTo(qreal angle)
{
    setZValue(10);
    setRotation(angle);
}
