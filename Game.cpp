#include "Game.h"
#include "Box.h"
#include <QKeyEvent>
#include <QMouseEvent> // не забудь подключить
Game::Game(QGraphicsScene *scene) : QGraphicsView(scene)
{
    tank = new Tank(scene);
    setFocus();
    movingUp = movingDown = rotatingLeft = rotatingRight = false;
}



void Game::mousePressEvent(QMouseEvent *event)
{
    if (tank) {
        tank->shoot(); // когда мышь нажали — танк стреляет
    }

    QGraphicsView::mousePressEvent(event); // если хочешь, чтобы мышь дальше работала
}
