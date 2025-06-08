
#pragma once
#include <QGraphicsPixmapItem>
#include <QObject>

class BonusBox : public QObject, public QGraphicsPixmapItem {
Q_OBJECT
public:
    enum BonusType { Health, Speed, Ammo };
    BonusBox(BonusType type);

    BonusType getType() const;

private:
    BonusType bonusType;
};
