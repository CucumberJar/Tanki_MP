#include "GameScene.h"
#include "Stone.h"
#include "../../Wall.h"

#include <QTimer>

#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>
#include <utility>
#include <QtWidgets>

GameScene::GameScene(QObject *parent) : QGraphicsScene(parent) {
    hudBackground = addRect(0, 0, 1000, 40, QPen(Qt::NoPen),
                            QBrush(QColor(20, 20, 20, 180)));
    hudBackground->setZValue(998);

    QFont hudFont("Arial", 12, QFont::Bold);
    healthText = addText("❤️ HP: 100", hudFont);
    ammoText = addText("💥 Ammo: 5", hudFont);
    playerCountText = addText("👥 Players: 1", hudFont);
    teamAScoreText = addText("🟥 Team A: 99", hudFont);
    teamBScoreText = addText("🟦 Team B: 100", hudFont);
    for (auto *t : {teamAScoreText, teamBScoreText})
        t->setDefaultTextColor(Qt::white),
                t->setZValue(999);
    QList<QGraphicsTextItem *> texts = {healthText, ammoText, playerCountText};
    for (auto *t : texts) {
        t->setDefaultTextColor(Qt::white);
        t->setZValue(999);
    }
}

void GameScene::onRemoveBlock(int x, int y) {
    auto* wall = new Wall(x,y);
    wall->setPos(x, y);
    wall->setZValue(10);
    this->addItem(wall);


    for (QGraphicsItem* item : this->collidingItems(wall)) {
        if (auto* stone = dynamic_cast<Stone*>(item)) {
            removeItem(stone);
            delete stone;
        }
    }
    this->removeItem(wall);
}



void GameScene::onDestroyTank(const QString &id) {
    if (!tanks.contains(id)) return;
    Tank* tank = tanks[id];
    tank->destroy();
    tanks.remove(id);
    updateHUD(0, 0);
}


void GameScene::spawnTank(const QString& id, qreal x, qreal y, bool isLocal) {
    if (tanks.contains(id)) {
        Tank* oldTank = tanks[id];
        tanks.remove(id);
        delete oldTank;
    }
    auto* tank = new Tank(isLocal, id, this->client);
    tank->setPos(x, y);
    tank->setRotation(0);
    addItem(tank);
    tanks[id] = tank;
    updateHUD(100, 20);

    if (isLocal) {
        localTank = tank;
        localPlayerId = id;
        sendTimer = new QTimer(this);
        connect(sendTimer, &QTimer::timeout, this, &GameScene::sendLocalTankPosition);
        sendTimer->start(20);
    }

}
void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton ) {
        if(localTank)
            localTank->shoot();
    }

    QGraphicsScene::mousePressEvent(event);
}
void GameScene::setView(QGraphicsView* view) {
    this->view = view;
}

void GameScene::setClient(QObject *clientObject) {
    client = qobject_cast<TankClient *>(clientObject);
    if (!client) return;
    connect(client, &TankClient::removeBlock, this, &GameScene::onRemoveBlock);
    connect(client, &TankClient::destroyTank, this, &GameScene::onDestroyTank);
    connect(client, &TankClient::spawnTank, this, &GameScene::onSpawnTank);
    connect(client, &TankClient::moveTank, this, &GameScene::onMoveTank);
}

void GameScene::onSpawnTank(const QString &id, int x, int y) {
    bool isLocal = (id == localPlayerId);
    spawnTank(id, x, y, isLocal);
}

void GameScene::onMoveTank(const QString &id, qreal x, qreal y, qreal angle, qreal tAngle){
    if (id == localPlayerId) return;
    if (!tanks.contains(id)) return;
    Tank *tank = tanks[id];
    tank->getTurret()->setRotation(tAngle);
    tank->setPos(x, y);
    tank->setRotation(angle);
}

void GameScene::sendLocalTankPosition() {
    if (!client || !tanks.contains(localPlayerId)) return;
    if (view) {
        QRectF visibleRect = view->mapToScene(view->viewport()->rect()).boundingRect();
        updateSceneRectAroundPlayer(visibleRect.width(), visibleRect.height());
    }
}

void GameScene::updateSceneRectAroundPlayer(qreal viewWidth, qreal viewHeight) {
    if (!tanks.contains(localPlayerId)) return;
    Tank* tank = tanks[localPlayerId];
    QPointF center = tank->pos();
    const qreal mapWidth = 60*32;
    const qreal mapHeight = 40*32;
    qreal halfW = viewWidth / 2;
    qreal halfH = viewHeight / 2;
    qreal x = center.x() - halfW;
    qreal y = center.y() - halfH;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + viewWidth > mapWidth) x = mapWidth - viewWidth;
    if (y + viewHeight > mapHeight) y = mapHeight - viewHeight;
    QRectF newRect(x, y, viewWidth, viewHeight);
    emit requestSceneRectChange(newRect);
    positionHUD();
}


void GameScene::updateHUD(int hp, int ammo) {
    if (healthText) healthText->setPlainText(QString("❤️ HP: %1").arg(hp));
    if (ammoText) ammoText->setPlainText(QString("💥 Ammo: %1").arg(ammo));
    if (playerCountText) playerCountText->setPlainText(QString("👥 Players: %1").arg(tanks.size()));
}
void GameScene::updateTeamScores(int teamAScore, int teamBScore) {
    if (teamAScoreText)
        teamAScoreText->setPlainText(QString("🟥 Team A: %1").arg(teamAScore));
    if (teamBScoreText)
        teamBScoreText->setPlainText(QString("🟦 Team B: %1").arg(teamBScore));
}

void GameScene::positionHUD() {
    if (!view || !hudBackground) return;

    QRectF visible = view->mapToScene(view->viewport()->rect()).boundingRect();
    qreal hudHeight = 40;
    qreal margin = 10;

    hudBackground->setRect(visible.left(), visible.bottom() - hudHeight, visible.width(), hudHeight);

    qreal leftX = visible.left() + margin;
    qreal rightX = visible.right() - margin;
    qreal centerX = visible.center().x();
    qreal y = visible.bottom() - hudHeight + margin;

    if (healthText)
        healthText->setPos(leftX, y);

    if (teamAScoreText)
        teamAScoreText->setPos(leftX + 100, y);

    if (ammoText) {
        QRectF bounds = ammoText->boundingRect();
        ammoText->setPos(centerX - bounds.width() / 2, y);
    }

    if (teamBScoreText) {
        QRectF bounds = teamBScoreText->boundingRect();
        teamBScoreText->setPos(rightX - 150-bounds.width(), y);
    }

    if (playerCountText) {
        QRectF bounds = playerCountText->boundingRect();
        playerCountText->setPos(rightX - bounds.width(), y);
    }
}

void GameScene::setBases(QVector<Base> bases) {
    this->bases=std::move(bases);
}

