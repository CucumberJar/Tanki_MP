#ifndef UNTITLED5_GAMELOAD_H
#define UNTITLED5_GAMELOAD_H
#include "map/Tile.h"
#include "map/Wall.h"
#include "../src/map/Base.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QJsonObject>
#include "GameView.h"
#include "GameScene.h"

class GameLoad {
    GameScene * scene;
    GameView * view;
    QMap<int, QPixmap> tilesetImages;

public:
    GameLoad(GameScene *scene, GameView *view);
    void loadMap(const QString &filename);
    void parseTilesets(const QJsonArray &tilesets);
    QPixmap getTilePixmap(int tileId);
};
#endif
