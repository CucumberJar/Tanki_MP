#ifndef TANK_H
#define TANK_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "../../../TankClient.h"


class Turret;

class Tank : public QObject, public QGraphicsPixmapItem {
Q_OBJECT

public:
    Tank(bool isLocal, const QString &id, TankClient *client = nullptr);
    QRectF boundingRect() const override;
    void shoot();
    bool isLocal() const { return isLocalTank; }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private slots:
    void move();

private:
    void updateTurretRotation();
    bool checkCollision();

    TankClient *client = nullptr;
    QString playerId;
    bool isLocalTank;
    Turret *turret = nullptr;
    bool movingUp = false;
    bool movingDown = false;
    bool rotatingLeft = false;
    bool rotatingRight = false;
    qreal speed = 2.0;
    qreal angle = 0.0;
};

#endif // TANK_H
