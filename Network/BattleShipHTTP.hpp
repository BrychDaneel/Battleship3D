#pragma once
#include "baseUDP.hpp"
#include <string>
#include <vector>

using std::vector;
using std::string;

namespace BattleShipHTTP{


struct ServerListItem{
    bool pass;
    unsigned int ID;
    string name;
    ServerListItem(const string _name,const unsigned int _ID,bool _pass);
};





}
