#ifndef TANK_H
#define TANK_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "../../../TankClient.h"

class TankClient;          // forward declaration


class Turret;

class Tank : public QObject, public QGraphicsPixmapItem {
Q_OBJECT



public:
    void destroy();
    Tank(bool isLocal, const QString &id, TankClient *client = nullptr);
    QRectF boundingRect() const override;
    void shoot();
    bool isLocal() const { return isLocalTank; }
    Turret * getTurret() {return turret;}
    int getHp(){return hp;}
    void setHp(int hp ){this->hp=hp;}
    QString getPlayerId(){return this->playerId;}


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private slots:
    void move();

private:
    void updateTurretRotation();
    bool checkCollision();
    int team;
    TankClient *client;
    QString playerId;
    bool isLocalTank;
    Turret *turret = nullptr;
    bool movingUp = false;
    bool movingDown = false;
    bool rotatingLeft = false;
    bool rotatingRight = false;
    qreal speed = 1.0;
    qreal angle = 0.0;
    QPointF lastSentPos;
    qreal lastSentAngle = -1;
    int hp=100;


    QTimer *shootCooldownTimer;
    bool canShoot;
    int lastSentTurretAngle;

};

#endif // TANK_H
