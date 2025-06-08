#pragma once

#include "TankClient.h"
#include "../src/MP/Playerinfo.h"
#include "GameScene.h"
#include "GameView.h"

class GameScene;           // forward declaration
class GameView;            // forward declaration
class TankClient;          // forward declaration


class Game : public QObject {
Q_OBJECT

public:
    explicit Game(const QString& nickname, const QString& serverIP, QObject* parent = nullptr);
    void start();


private:
    QString nickname;
    QString serverIP;
    TankClient* client = nullptr;
    GameScene* scene;
    GameView* view;
};
