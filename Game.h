#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "Tank.h"

class Game : public QGraphicsView
{
Q_OBJECT

public:
    Game(QGraphicsScene *scene);

private:
    Tank *tank;

    // Флаги для отслеживания нажатых клавиш
    bool movingUp;
    bool movingDown;
    bool rotatingLeft;
    bool rotatingRight;

    void advance(int phase);
};

#endif // GAME_H
