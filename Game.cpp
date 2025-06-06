#include "Game.h"
#include "GameLoad.h"
#include "tank/entities/Tank.h"
#include "TankClient.h"
#include <QObject> // 🔥 обязательно, чтобы работал connect


Game::Game(const QString& nickname, const QString& serverIP, QObject* parent)
        : QObject(parent), nickname(nickname), serverIP(serverIP)
{
    scene = new GameScene();
    view = new GameView(scene);
}


void Game::start() {
    auto* client = new TankClient(this);
    connect(client, &TankClient::spawnTank, scene, &GameScene::onSpawnTank);

    client->connectToServer(serverIP, 12345);

    // генерируем уникальный ID
    QString playerId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    client->sendJoinMessage(playerId);
    scene->setLocalPlayerId(playerId);
    GameLoad loader(scene, view);
    view->show();
}