#pragma once
#include <string>

using std::string;

namespace BattleShipOptions{
    extern string Nick,EnemyNick;
    extern unsigned ClientID, EnamiID;
    extern bool Turn, NotSet;


extern  string MasterIP, MasterHost;
extern unsigned short MasterPort;
extern string Page_List, Page_Add, Page_Connect, Page_Disconnect, \
    Page_Action, Page_Message;

extern string OgreResourcesCfg, OgrePluginsCfg;

    void InitOptions();
}
