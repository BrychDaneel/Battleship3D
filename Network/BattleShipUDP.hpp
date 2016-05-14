#pragma once
#include <string>
#include <vector>

using std::vector;
using std::string;



namespace BattleShipUDP{

extern unsigned short port;


extern bool connected;

extern const int timeOut, maxRetry;

struct ServerListItem{
    string ip;
    unsigned short port;
    bool quick, fav, pass;
    string name;

    ServerListItem(const string _name,const string _ip,const unsigned short _port,bool _quick,bool _fav,bool _pass);
};

extern vector<ServerListItem> ServerList;

void Connect(const int ServerID);
void Connect(const int ServerID, const string password);
void Disconnect();

void Search();

void Say(const string message);
void Say(const char *message);

void OpenServer();
void OpenServer(const string Name);
void OpenServer(const string Name, const string pass);
void CloseServer();

void AddFavorite(const string ip, const unsigned short port);
void AddFavorite(const string ip);
void DelFavorite(const string ip, const unsigned short port);
void DelFavorite(const string ip);
void ClearFavorites();

void SetBattle(const bool battle[10][10]);
void Attack(const unsigned char x,const unsigned char y);

void initUdpProtocol(void (*__onFoundServer)(const int id),void (*__onConnect)(),void (*__onConnected)(),void (*__onDisConnect)(),void (*__onInvalid)(),void (*__onRefus)(), \
        void (*__onSet)(const bool battle[10][10]),  void (*__onAttack)(const unsigned char x,const unsigned char y), void (*__onChatIn)(const string message));

}

