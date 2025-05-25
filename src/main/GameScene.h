#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QString>
#include "tank/entities/Tank.h"

class GameScene : public QGraphicsScene {
Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

    void spawnTank(const QString& id, qreal x, qreal y, bool isLocal);
    void onSpawnTank(const QString &id, int x, int y);
    void onMoveTank(const QString &id, qreal x, qreal y, qreal angle);
    void setLocalPlayerId(const QString &id) { localPlayerId = id; }
    void setClient(QObject *clientObject);  // указатель на TankClient

private slots:

    void sendLocalTankPosition();

private:
    QMap<QString, Tank*> tanks;
    QString localPlayerId;
    class TankClient *client = nullptr;
    QTimer *sendTimer = nullptr;
};

#endif // GAMESCENE_H
