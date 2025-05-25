#include "GameScene.h"
#include <QTimer>
#include <QDebug>

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent) {}

void GameScene::spawnTank(const QString& id, qreal x, qreal y, bool isLocal) {
    qDebug() << "[GameScene] Spawning tank with id:" << id << "at" << x << y << "local:" << isLocal;

    if (tanks.contains(id)) return;  // уже есть

    auto* tank = new Tank(isLocal, id);
    tank->setPos(x, y);
    tank->setRotation(0);
    addItem(tank);
    tanks[id] = tank;

    if (isLocal) {
        // запускаем таймер отправки положения
        sendTimer = new QTimer(this);
        connect(sendTimer, &QTimer::timeout, this, &GameScene::sendLocalTankPosition);
        sendTimer->start(50);  // 20 раз в секунду
    }
}

void GameScene::setClient(QObject *clientObject) {
    client = qobject_cast<TankClient *>(clientObject);
    if (!client) return;

    connect(client, &TankClient::spawnTank, this, &GameScene::onSpawnTank);
    connect(client, &TankClient::moveTank, this, &GameScene::onMoveTank);
}

void GameScene::onSpawnTank(const QString &id, int x, int y) {
    bool isLocal = (id == localPlayerId);
    spawnTank(id, x, y, isLocal);
    qDebug() << "SPAWN id:" << id << "x:" << x << "y:" << y << "isLocal:" << isLocal;

}

void GameScene::onMoveTank(const QString &id, qreal x, qreal y, qreal angle) {
    if (id == localPlayerId) return;
    if (!tanks.contains(id)) return;

    Tank *tank = tanks[id];
    tank->setPos(x, y);
    tank->setRotation(angle);
}

void GameScene::sendLocalTankPosition() {
    if (!client || !tanks.contains(localPlayerId)) return;

    Tank *tank = tanks[localPlayerId];
    QPointF pos = tank->pos();
    qreal angle = tank->rotation();

    QString msg = QString("MOVE;%1;%2;%3;%4\n")
            .arg(localPlayerId)
            .arg(pos.x())
            .arg(pos.y())
            .arg(angle);

    client->sendRawMessage(msg);  // должна быть функция отправки сырого текста
}
