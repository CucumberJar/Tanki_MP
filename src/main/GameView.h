#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include "GameScene.h"
class GameScene;
class GameView : public QGraphicsView {
    GameScene* scene;

public slots:
    void onSceneRectChange(const QRectF& rect);
public:
    GameView(GameScene* scene);
    void load(int mapWidth, int tileWidth, int mapHeight, int tileHeight);
};

#endif
