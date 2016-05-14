#include "BattleShipHTTP.hpp"

#include "baseHTTP.hpp"

namespace BattleShipHTTP{

HTTPClient *H;

ServerListItem::ServerListItem(const string _name,const unsigned int _ID,bool _pass){
    name=_name;
    ID=_ID;
    pass=_pass;
}

void Init(){
}

}
