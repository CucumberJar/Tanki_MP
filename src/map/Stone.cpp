#include "Stone.h"
Stone::Stone(qreal x, qreal y) {
    setPos((x) * 32, (y) * 32);
    QPixmap boxPixmap("../drawable/texture/stone.png");
    setPixmap(boxPixmap);
}