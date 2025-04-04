#include "Turret.h"
#include <QPixmap>
#include <QTransform>

Turret::Turret(QGraphicsItem *parent)
        : QObject(), QGraphicsPixmapItem(parent)
{
    QPixmap turretPixmap("D:/untitled5/drawable/tower.png");
    setPixmap(turretPixmap);

    // Центр вращения — центр изображения
    setTransformOriginPoint(47, 32);
}

void Turret::rotateTo(qreal angle)
{
    setRotation(angle);
}
