#include "QGraphicsScene"
#include <QGraphicsView>
#include <QJsonObject>
#include "GameView.h"


GameView::GameView(GameScene* scene) : QGraphicsView(scene) {
    this->scene=scene;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHint(QPainter::TextAntialiasing, true);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
    setFocus();
    showFullScreen();
}
void GameView::load(int mapWidth,int  tileWidth, int mapHeight, int tileHeight){
    scene->setSceneRect(0, 0, mapWidth * tileWidth, mapHeight * tileHeight);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

