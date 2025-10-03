#ifndef TILE_H
#define TILE_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
class Tile : public QGraphicsPixmapItem {
public:
    Tile(int x, int y, const QPixmap &pixmap);
};
#endif
