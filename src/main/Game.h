#pragma once
#include "TankClient.h"
#include "GameScene.h"
#include "GameView.h"
class GameScene;
class GameView;
class TankClient;

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
