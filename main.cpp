#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMouseEvent>
#include "Tank.h"

class CustomView : public QGraphicsView {
public:
    Tank *tank;

    explicit CustomView(Tank *t, QGraphicsScene *scene) : QGraphicsView(scene), tank(t) {
        setMouseTracking(true);
    }

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        if (tank) {
            QPointF scenePos = mapToScene(event->pos());
            tank->rotateTurret(scenePos); // Новый метод для вращения башни
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (tank) {
            tank->setFocus(); // Возвращаем фокус, чтобы танк снова слушал клавиатуру
        }
        QGraphicsView::mousePressEvent(event);
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QGraphicsScene *scene = new QGraphicsScene();
    Tank *tank = new Tank();
    tank->setPos(100, 100);

    scene->addItem(tank);
    tank->setFlag(QGraphicsItem::ItemIsFocusable);
    tank->setFocus();

    CustomView view(tank, scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setFixedSize(1000, 1000);
    view.show();

    qDebug() << "Game started!";

    return a.exec();
}
