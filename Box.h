#ifndef BOX_H
#define BOX_H

#include <QGraphicsPixmapItem>

class Box : public QGraphicsPixmapItem {
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    Box(qreal x, qreal y);
};

#endif // BOX_H
