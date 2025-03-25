#ifndef TURRET_H
#define TURRET_H

#include <QGraphicsRectItem>
#include <QTimer>

class Turret : public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    explicit Turret(QGraphicsItem* parent = nullptr);
    void rotateToCursor(QPointF cursorPos); // Устанавливаем целевой угол

private slots:
    void updateRotation(); // Метод для плавного поворота

private:
    qreal targetAngle = 0; // Целевой угол поворота
    QTimer* rotationTimer; // Таймер для плавного поворота
};

#endif // TURRET_H
