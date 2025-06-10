#include "Tile.h"
Tile::Tile(int x, int y, const QPixmap &pixmap) {
    setPixmap(pixmap);
    setPos(x, y);
}
