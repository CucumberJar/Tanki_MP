#include "QGraphicsScene"
#include <QGraphicsView>
#include <QJsonObject>
#include "GameView.h"

GameView::GameView(GameScene* scene) : QGraphicsView(scene) {
    this->scene = scene;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing, false);
    setRenderHint(QPainter::SmoothPixmapTransform, true);  // ← отключи
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    setRenderHint(QPainter::TextAntialiasing, true);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
   // setFocus();
    showFullScreen();

    connect(scene, &GameScene::requestSceneRectChange, this, &GameView::onSceneRectChange);
}

void GameView::load(int mapWidth, int tileWidth, int mapHeight, int tileHeight) {
    scene->setSceneRect(0, 0, mapWidth * tileWidth, mapHeight * tileHeight);

    // Отображаем фиксированную область вокруг игрока (например, 25x18 тайлов)
    QRectF targetView(0, 0, tileWidth * 22, tileHeight * 20); // ≈ 800x576, зависит от тайла
    fitInView(targetView, Qt::KeepAspectRatio);
}


void GameView::onSceneRectChange(const QRectF& rect) {
    setSceneRect(rect);
    centerOn(rect.center());
}

