#ifndef TANK_H
#define TANK_H

#include <QGraphicsRectItem>
#include <QTimer>
#include "Turret.h"

class Tank : public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    Tank();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event); // Добавляем обработку мыши
public:
    void rotateTurret(const QPointF &cursorPosition);

private slots:
    void updateMovement();

private:
    QTimer* moveTimer;
    bool isMoving = false;
    bool isRotatingLeft = false;
    bool isRotatingRight = false;
    const qreal speed = 5.0;
    const qreal rotationSpeed = 5.0;

    Turret* turret; // Башня
};

#endif // TANK_H
