#include "Turret.h"
#include <QPixmap>
#include <QTransform>

Turret::Turret(QGraphicsItem *parent)
        : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap turretPixmap("D:/untitled5/drawable/turret.png");
    setPixmap(turretPixmap);
    setTransformOriginPoint(turretPixmap.width()/2, turretPixmap.height()/2);
}

void Turret::rotateTo(qreal angle)
{
    setRotation(angle);
}
