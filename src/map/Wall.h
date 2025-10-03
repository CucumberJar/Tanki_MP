#ifndef BOX_H
#define BOX_H
#include <QGraphicsPixmapItem>
class Wall : public QGraphicsPixmapItem {
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    Wall(qreal x, qreal y);
    void setImage(QString url);
};
#endif
