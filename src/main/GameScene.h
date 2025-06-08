#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <QString>
#include <QGraphicsTextItem>
#include <QTimer>
#include "tank/entities/Tank.h"
#include "Base.h"

class Tank;

class GameScene : public QGraphicsScene {
Q_OBJECT
public:
    void updateTeamScores(int teamAScore, int teamBScore);
    explicit GameScene(QObject *parent = nullptr);
    void setView(QGraphicsView* view);
    void spawnTank(const QString& id, qreal x, qreal y, bool isLocal);
    void onSpawnTank(const QString &id, int x, int y);
    void onMoveTank(const QString &id, qreal x, qreal y, qreal angle);
    void setLocalPlayerId(const QString &id) { localPlayerId = id; }
    void setClient(QObject *clientObject);
    void positionHUD();
    void setBases(QVector<Base> bases);
signals:
    void requestSceneRectChange(const QRectF& rect);

public slots:
    void updateSceneRectAroundPlayer(qreal viewWidth, qreal viewHeight);
    void updateHUD(int hp, int ammo);
    void sendLocalTankPosition();

private:
    QGraphicsTextItem *teamAScoreText = nullptr;
    QGraphicsTextItem *teamBScoreText = nullptr;
    QGraphicsRectItem *hudBackground = nullptr;
    QGraphicsView* view = nullptr;
    QMap<QString, Tank*> tanks;
    QString localPlayerId;
    class TankClient *client = nullptr;
    QTimer *sendTimer = nullptr;
    QGraphicsTextItem *healthText = nullptr;
    QGraphicsTextItem *ammoText = nullptr;
    QGraphicsTextItem *playerCountText = nullptr;
    QVector<Base> bases;



    Tank *localTank;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);


    void startBonusSpawner();

    void spawnBonus();
};

#endif // GAMESCENE_H
