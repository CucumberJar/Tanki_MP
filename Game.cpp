#include "Game.h"
#include "GameLoad.h"
#include "tank/entities/Tank.h"
#include "TankClient.h"
#include <QObject>
#include <QUuid>
#include <QDebug>

Game::Game() {
    scene = new GameScene();
    view = new GameView(scene);
}

void Game::start() {
    auto* client = new TankClient(this);

    // Подключаем клиент к сцене
    scene->setClient(client);

    // Подключение только одного сигнала недостаточно!
    connect(client, &TankClient::spawnTank, scene, &GameScene::onSpawnTank);
    connect(client, &TankClient::moveTank, scene, &GameScene::onMoveTank);

    client->connectToServer("192.168.1.2", 12345);

    // Генерируем уникальный ID
    QString playerId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    client->sendJoinMessage(playerId);

    scene->setLocalPlayerId(playerId);

    GameLoad loader(scene, view);
    view->show();
}
