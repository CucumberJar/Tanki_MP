#include "Game.h"
#include "TankClient.h"
#include "GameLoad.h"
#include <QDebug>

Game::Game(const QString& nickname, const QString& serverIP, QObject* parent)
        : QObject(parent), nickname(nickname), serverIP(serverIP)
{
    scene = new GameScene();
    view = new GameView(scene);
    scene->setView(view);
}

void Game::start() {
    auto* client = new TankClient(this);
    connect(client, &TankClient::spawnTank, scene, &GameScene::onSpawnTank);
    client->connectToServer(serverIP, 12345);

    QString playerId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    client->sendJoinMessage(playerId);
    scene->setClient(client);
    scene->setLocalPlayerId(playerId);

    GameLoad loader(scene, view);
    view->show();
}

