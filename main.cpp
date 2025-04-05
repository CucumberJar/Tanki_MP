#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Game.h"
#include "Box.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene scene;
    scene.addItem(new Box(0, 0));
    scene.addItem(new Box(32, 0));
    scene.addItem(new Box(64, 0));
    scene.addItem(new Box(0, 32));
    scene.addItem(new Box(0,64 ));
    scene.addItem(new Box(300, 300));
    scene.addItem(new Box(300, 400));
    scene.addItem(new Box(300, 500));
    scene.addItem(new Box(500, 200));
    scene.setSceneRect(0, 0, 800, 600); // размер сцены 800x600

    scene.addItem(new Box(400, 400));
    Game gameWindow(&scene);
    gameWindow.show();
    return QApplication::exec();
}
