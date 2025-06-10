#include <QtWidgets>
#include "GameLoad.h"
#include "GameScene.h"
#include "Stone.h"


GameLoad::GameLoad(GameScene * scene, GameView * view) {
    this->scene = scene;
    this->view=view;
    QString map = "../desert2.tmj";
    loadMap(map);
}

void GameLoad::loadMap(const QString &filename) {

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл карты: " << filename;
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject mapData = doc.object();

    int mapWidth = mapData["width"].toInt();
    int mapHeight = mapData["height"].toInt();
    int tileWidth = mapData["tilewidth"].toInt();
    int tileHeight = mapData["tileheight"].toInt();

    qDebug() << "Загружаем карту с размерами: " << mapWidth << "x" << mapHeight;
    QJsonArray tilesets = mapData["tilesets"].toArray();
    parseTilesets(tilesets);
    view->load(mapWidth,tileWidth,mapHeight+2,mapHeight);
    QJsonArray layers = mapData["layers"].toArray();
    for (const QJsonValue &layerValue : layers) {
        QJsonObject layer = layerValue.toObject();
        QJsonArray data = layer["data"].toArray();

        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int index = (y * mapWidth) + x;
                if (index < data.size()) {
                    int tileId = data[index].toInt();
                    if (tileId > 0) {
                        QPixmap tilePixmap = getTilePixmap(tileId);
                        if (tileId == 3) {
                            Wall *wall = new Wall(x, y);
                            scene->addItem(wall);
                        }
                        if (tileId==15){
                            Base *base = new Base(x,y);
                            base->setZValue(layer["id"].toInt() - 2);
                            scene->addItem(base);
                        } else
                        if (tileId==2){
                            Stone *stone = new Stone(x,y);
                            stone->setZValue(layer["id"].toInt() - 2);
                            scene->addItem(stone);
                        } else
                        if (!tilePixmap.isNull()&&tileId!=3&&tileId!=2) {
                            Tile *tile = new Tile(x * tileWidth, y * tileHeight, tilePixmap);
                            tile->setZValue(layer["id"].toInt() - 2);
                            scene->addItem(tile);
                        }
                    }
                }
            }
        }
    }
}

void GameLoad::parseTilesets(const QJsonArray &tilesets) {
    for (const QJsonValue &tilesetValue : tilesets) {
        QJsonObject tileset = tilesetValue.toObject();
        int firstGid = tileset["firstgid"].toInt();
        QString imagePath = tileset["image"].toString();

        QPixmap tilePixmap(imagePath);
        if (!tilePixmap.isNull()) {
            tilesetImages.insert(firstGid, tilePixmap);
        } else {
            qWarning() << "Не удалось загрузить изображение тайла: " << imagePath;
        }
    }
}

QPixmap GameLoad::getTilePixmap(int tileId) {
    return tilesetImages.value(tileId, QPixmap());
}
