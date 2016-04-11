#pragma once
#include <string>

using std::string;

namespace BattleShipOptions{
    extern string Nick,EnemyNick;
    extern unsigned ClientID, EnamiID;
    extern bool Turn, NotSet;

    void InitOptions();
}
