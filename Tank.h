#ifndef TANK_H
#define TANK_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>

class Tank : public QObject, public QGraphicsPixmapItem
{
Q_OBJECT
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    Tank(QGraphicsScene *scene);
    void moveUp();
    void moveDown();
    void rotateLeft();
    void rotateRight();
    void updateMovement();
    void setRotationSpeed(double speed);

private:
    double rotationSpeed{};
    double angle;
    int speed;
    bool movingUp;
    bool rotatingLeft;
    bool rotatingRight;
    bool movingDown;

    void keyReleaseEvent(QKeyEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void move();

    bool checkCollision();
};

#endif // TANK_H
