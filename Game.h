#pragma once
#include <QObject>
#include "GameScene.h"
#include "GameView.h"

class Game : public QObject {
Q_OBJECT
public:
    explicit Game(const QString& nickname, const QString& serverIP, QObject *parent = nullptr);
    void start();

private:
    QString nickname;
    QString serverIP;

    GameScene *scene;
    GameView *view;
};
