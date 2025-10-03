#ifndef TURRET_H
#define TURRET_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsSceneHoverEvent>
class Turret : public QObject, public QGraphicsPixmapItem {
Q_OBJECT
public:
    Turret(QGraphicsItem *parent = nullptr);
    void rotateTo(qreal angle);
};
#endif
