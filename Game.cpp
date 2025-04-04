#include "Game.h"
#include "Box.h"
#include <QKeyEvent>

Game::Game(QGraphicsScene *scene) : QGraphicsView(scene)
{
    tank = new Tank(scene);
    setFocus();
    movingUp = movingDown = rotatingLeft = rotatingRight = false;
}

