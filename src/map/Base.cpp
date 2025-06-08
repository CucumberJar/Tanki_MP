
#include "Base.h"

Base::Base(qreal x, qreal y) {
    setPos((x) * 32, (y - 1) * 32);
    QPixmap boxPixmap("../drawable/texture/base.png");
    setPixmap(boxPixmap);
}

void Base::setTeam(int team) {
    if (team >= 0) {
        this->team = team;
        switch (team) {
            case 0: {
                QPixmap boxPixmap("../drawable/texture/base1.png");
                setPixmap(boxPixmap);
                break;
            }
            case 1: {
                QPixmap boxPixmap1("../drawable/texture/base2.png");
                setPixmap(boxPixmap1);
                break;
            }

        }
    }

}

int Base::getTeam(){
    return this->team;
}

void Base::setId(int id) {
    this->id=id;
}

int Base::getId() const {
    return this->id;
}
