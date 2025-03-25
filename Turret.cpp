#include "Turret.h"
#include <QtMath>
#include <QDebug>

Turret::Turret(QGraphicsItem* parent) : QGraphicsRectItem(parent) {
    setRect(0, 0, 40, 40); // Размеры башни
    setTransformOriginPoint(20, 20); // Центр башни

    // Таймер для плавного поворота
    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &Turret::updateRotation);
    rotationTimer->start(16); // 60 FPS (каждые 16 мс)
}

void Turret::rotateToCursor(QPointF cursorPos) {
    // Вычисляем целевой угол, но не поворачиваем мгновенно
    QPointF turretCenter = mapToScene(transformOriginPoint());
    targetAngle = qRadiansToDegrees(qAtan2(cursorPos.y() - turretCenter.y(), cursorPos.x() - turretCenter.x()));
}

void Turret::updateRotation() {
    qreal currentAngle = rotation();
    qreal diff = targetAngle - currentAngle;

    // Учитываем кратчайший путь вращения (не крутим через 360°)
    if (diff > 180) diff -= 360;
    if (diff < -180) diff += 360;

    // Плавный поворот: ограничиваем скорость до 2° за тик
    qreal rotationStep = 2.0;
    if (qAbs(diff) > rotationStep) {
        setRotation(currentAngle + (diff > 0 ? rotationStep : -rotationStep));
    } else {
        setRotation(targetAngle); // Завершаем поворот
    }
}
