#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Tile : public QGraphicsPixmapItem {
public:
    // Конструктор класса
    Tile(int x, int y, const QPixmap &pixmap);

    // Дополнительные методы, если они понадобятся
    // Например, можно добавить метод для получения позиции:
    // QPointF getPosition() const;
};

#endif // TILE_H
