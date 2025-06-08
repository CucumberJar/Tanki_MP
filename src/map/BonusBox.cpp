#include "BonusBox.h"

BonusBox::BonusBox(BonusType type) : bonusType(type) {
    setPixmap(QPixmap("../drawable/texture/hpBox.png"));
    setZValue(10);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

BonusBox::BonusType BonusBox::getType() const {
    return bonusType;
}
