#pragma once

#include <QGraphicsItem>
#include <QTimer>

class Bullet : public QObject, public QGraphicsItem
{
Q_OBJECT
public:
    Bullet(qreal angle);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void move();

private:
    qreal angle;
    qreal speed;
    qreal radius;
};
