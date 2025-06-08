#pragma once
#include <QString>

struct PlayerInfo {
    QString id;
    QString nickname;
    bool inBattle = false;
    int kills = 0;
    int deaths = 0;
};
