#pragma once

#include <QGraphicsItem>
#include <QTimer>
#include "TankClient.h"
class TankClient;
class Bullet : public QObject, public QGraphicsItem
{
Q_OBJECT
public:
    Bullet(qreal angle);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    const QString &getPlayerId() const;
    void setPlayerId(const QString &playerId);
    void setClient(TankClient * client){ this->client=client;}

public slots:
    void move();

private:
    TankClient* client;
    qreal angle;
    qreal speed;
    qreal radius;
    QString playerId;
    bool checkCollision();
};
