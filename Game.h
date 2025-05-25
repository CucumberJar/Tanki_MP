#pragma once
#include <QObject>
#include "GameScene.h"
#include "GameView.h"

class Game : public QObject {
Q_OBJECT  // обязательно, если будут сигналы/слоты
public:
    Game();
    void start();
private:
    GameScene *scene;
    GameView *view;
};
