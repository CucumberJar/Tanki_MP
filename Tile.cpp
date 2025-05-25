#include "Tile.h"

// Конструктор класса
Tile::Tile(int x, int y, const QPixmap &pixmap) {
    setPixmap(pixmap);  // Устанавливаем изображение
    setPos(x, y);       // Устанавливаем позицию
}
