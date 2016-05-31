#include "BattleShipUDP.hpp"

#include "baseUDP.hpp"
#include "../Utility/timer.hpp"
#include "../Utility/cipher.hpp"
#include "../Game/options.hpp"


#include <string>
#include <queue>
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <sstream>

using namespace BattleShipOptions;
using std::vector;
using std::queue;
using std::string;
using std::ostringstream;
using std::istringstream;
using std::stringstream;





namespace BattleShipUDP{

ServerListItem::ServerListItem(const string _name,const string _ip,const unsigned short _port,bool _quick, bool _fav,bool _pass){
    name=_name;
    ip=_ip;
    port=_port;
    quick=_quick;
    fav=_fav;
    pass=_pass;
}

static UDPServer* server;
static timer *atimer;
vector<ServerListItem> ServerList;

struct FavoriteItem{
    string ip;
    unsigned short port;
    FavoriteItem(const string _ip,const unsigned short _port){
        ip=_ip;
        port=_port;
    }
};

vector<FavoriteItem> Favorites;

unsigned short port;
string SendIP;
unsigned short SendPort;

queue<string> reqvestQueue;
unsigned packegeID, catchID;
unsigned sendtime;
string ServerName;

bool wantConnect=false,connected=false, wantQuick=false;
string MyPass;

const int timeOut=500, pingTime=5000, maxRetry=3;
const unsigned short standartPort=15565;

int retry=0;


void (*_onFoundServer)(const int id),(*_onConnect)(), (*_onConnected)(), (*_onDisConnect)(),(*_onInvalid)(),(*_onRefuse)(), (*_onSet)(const bool battle[10][10]),  \
         (*_onAttack)(const unsigned char x,const unsigned char y),  (*_onChatIn)(const string message);

string getline(istringstream & stream){
    string q;
    getline(stream,q);
    return q;
}

string GetString(istringstream & stream){
    string q;
    stream>>q;
    return q;
}


string SkipGetLine(istringstream & stream){
    string q;
    char sp;
    stream.get(sp);
    getline(stream,q);
    return q;
}

string uinttostr(const unsigned a){
    ostringstream stream;
    stream<<a;
    return stream.str();
}

unsigned strtouint(const string s){
    istringstream stream(s);
    unsigned a;
    stream>>a;
    return a;
}

string Packege(const int id,const string s){
    ostringstream stream;
    stream<<"safe "<<id<<' '<<s;
    return stream.str();
}

string parcePackege(const string pack,unsigned &outId){
        std::istringstream stream(pack);
        string q;
        stream>>q>>outId;
        return getline(stream);
}


void addServer(const bool passed,const string name, const string ip,const unsigned short port,bool quick){
    bool isfav=false;
    for (unsigned int i=0; i<Favorites.size(); i++)
        isfav=isfav || (Favorites[i].ip==ip && Favorites[i].port==port);

    ServerList.push_back(ServerListItem(name, ip, port, quick,isfav,passed));
    (_onFoundServer)(ServerList.size()-1);
}

void response(const string ip,const unsigned short port){

    if (wantQuick)
        server->send(ip, port, "IamQuickServer");
    else
        server->send(ip, port, (string)"IamServer "+(MyPass==""?"nopass ":"pass ")+ServerName);
}

void chatIn(string s){
    (*_onChatIn)(s);
}

void Send(const string s);

void onConnected(const unsigned ID, const string _Nick, const string ip,const unsigned short port){
        if (!wantConnect)
            return;

        SendIP=ip;
        SendPort=port;
        EnamiID=ID;
        EnemyNick=_Nick;
        wantConnect=false;
        connected=true;

        Send("sucscon "+uinttostr(ClientID)+" "+Nick);

        (*_onConnected)();
}

void onConnect(const unsigned ID,  const string _Nick){
    EnamiID=ID;
    EnemyNick=_Nick;
    connected=true;
    wantConnect=false;

    (*_onConnect)();
}

void ClearConnection(){
    connected=false;
    NotSet=true;
}

void onDisconnect(){
    ClearConnection();
    (*_onDisConnect)();
}

void onAttack(unsigned char x, unsigned char y){
    (*_onAttack)(x,y);
}

void onSet(const string battle){
    NotSet=false;
    istringstream stream(battle);
    bool a[10][10];
    for (int i=0; i<10; i++)
        for (int ii=0; ii<10; ii++){
            char c;
            stream>>c;
            a[i][ii]=c-'0';
        }
    bool b[10][10];
    unciphing(EnamiID,a,b);

    (*_onSet)(b);
}

void readComand(const string message,const string ip,const unsigned short port){
        std::istringstream stream(message);
        string s;
        stream>>s;

        if (s=="search" && wantConnect)
            response(ip,port);

        if (s=="IamServer"){
            bool passed=GetString(stream)=="pass";
            addServer(passed,SkipGetLine(stream),ip,port,false);
        }
        if (s=="IamQuickServer")
            addServer("","",ip,port,true);

        if (s=="connect" && !connected){
            if (MyPass!="" && GetString(stream)!=MyPass){
                server->send(ip, port, "invalid");
                return;
                }
            unsigned id=strtouint(GetString(stream));
            onConnected(id,SkipGetLine(stream),ip,port);
        }


        if (SendIP==ip && SendPort==port && !connected){

        if (s=="invalid")
            (*_onInvalid)();

        if (s=="sucscon"){
            unsigned id=strtouint(GetString(stream));
            onConnect(id,getline(stream));
        }

        }

        if (SendIP==ip && SendPort==port && connected){

        if (s=="disconnect" && SendIP==ip && SendPort==port)
            onDisconnect();

        if (s=="say")
            chatIn(SkipGetLine(stream));

        if (s=="set" && NotSet)
            onSet(SkipGetLine(stream));

        if (s=="attack" && !NotSet && !Turn){
            int x=strtouint(GetString(stream));
            onAttack(x,strtouint(GetString(stream)));
        }

        }
}

void safeReadData(const string message,const string ip,const unsigned short  port){
    unsigned pid;
    string newMessage;
    newMessage=parcePackege(message,pid);
    std::ostringstream stream;
    stream<<"OK ";
    stream<<pid;
    server->send(ip,port,stream.str());

    if (pid!=catchID)
        readComand(newMessage,ip,port);
    catchID=pid;
}
void SendNext();

void getOK(unsigned pid){
    if (pid!=packegeID)
        return;
    retry=0;
    sendtime=INT_MAX;
    reqvestQueue.pop();
    packegeID++;
    SendNext();

}

void readData(const string message,const string ip,const unsigned short  port){

        std::istringstream stream(message);
        string s;
        stream>>s;
        if (s=="OK"){
            unsigned pid;
            stream>>pid;
            getOK(pid);
        }
        else
        if (s=="safe")
            safeReadData(message,ip,port);
        else
            readComand(message,ip,port);
}

void readData(const char* buffer,const size_t size,const string ip,const unsigned short  port){
    readData(string(buffer,0,size),ip,port);
}

void SendNext(){
    if (reqvestQueue.size()==0)
        return;
    server->send(SendIP,SendPort,Packege(packegeID,reqvestQueue.front()));
    sendtime=clock();
}

void Send(const string s){
    reqvestQueue.push(s);
    if (reqvestQueue.size()==1)
        SendNext();
}

int lastping=0;
void TimeOutTime(){
        if (connected && clock()-lastping>pingTime)
            Send("ping");


        if (reqvestQueue.size()>0 && clock()-sendtime>timeOut){
            retry++;
            if (retry>=maxRetry){
                while (reqvestQueue.size())
                    reqvestQueue.pop();
                packegeID++;
                ClearConnection();
                (_onRefuse)();
            }
            else
                SendNext();
        };
}


void Connect(const string ip,const unsigned short port, const string password){
            if (connected)
                return;
            CloseServer();
            SendIP=ip;
            SendPort=port;
            if (password=="")
                Send("connect "+uinttostr(ClientID)+" "+Nick);
            else
                Send("connect "+password+" "+uinttostr(ClientID)+" "+Nick);
}

void Connect(const int ServerID){
    if (ServerList[ServerID].pass)
        return;
    Connect(ServerList[ServerID].ip,ServerList[ServerID].port, "");
}

void Connect(const int ServerID, const string password){
    if (!ServerList[ServerID].pass)
        return;
    Connect(ServerList[ServerID].ip,ServerList[ServerID].port, password);
}

void Disconnect(){
    if (!connected)
        return;
    ClearConnection();
    Send("disconnect");
}

void Search(){
    ServerList.clear();
    server->send(standartPort,"search");
    for (unsigned int i=0; i<Favorites.size(); i++)
        server->send(Favorites[i].ip,Favorites[i].port,"search");
}

void AddFavorite(const string ip, const unsigned short port){
    Favorites.push_back(FavoriteItem(ip,port));
}

void AddFavorite(const string ip){
    AddFavorite(ip,standartPort);
}

void DelFavorite(const string ip, const unsigned short port){
    for (unsigned int i=0; i<Favorites.size(); i++)
        if (Favorites[i].ip==ip && Favorites[i].port==port)
            Favorites.erase(Favorites.begin()+i--);
}

void DelFavorite(const string ip){
    DelFavorite(ip,standartPort);
}

void ClearFavorites(){
    Favorites.clear();
}

void Say(const string message){
    if (!connected)
        return;
    Send("say "+message);
}

void Say(const char *message){
    if (!connected)
        return;
    string s(message);
    s="say "+s;
    Send(s);
}

void OpenServer(){
    if (connected)
        return;
    MyPass="";
    wantConnect=true;
    wantQuick=true;
}

void OpenServer(const string Name){
    MyPass="";
    wantConnect=true;
    wantQuick=false;
    ServerName=Name;
}


void OpenServer(const string Name, const string pass){
    MyPass=pass;
    wantConnect=true;
    wantQuick=false;
    ServerName=Name;
}

void CloseServer(){
    wantConnect=false;
}

void SetBattle(const bool battle[10][10]){
    if (!connected)
        return;
    bool a[10][10];
    ciphing(ClientID,battle,a);
    ostringstream stream;

    stream<<"set ";
    for (int i=0; i<10; i++)
        for (int ii=0; ii<10; ii++)
            stream<<(a[i][ii]?1:0);

    Send(stream.str());
}

void Attack(const unsigned char x,const unsigned char y){
        if (!connected)
            return;
        Send("attack "+uinttostr(x)+" "+uinttostr(y));
}

void initUdpProtocol(void (*__onFoundServer)(const int id),void (*__onConnect)(),void (*__onConnected)(),void (*__onDisConnect)(),void (*__onInvalid)(), void (*__onRefus)(), \
        void (*__onSet)(const bool battle[10][10]),  void (*__onAttack)(const unsigned char x,const unsigned char y), void (*__onChatIn)(const string message)){

    _onFoundServer=__onFoundServer;
    _onConnect=__onConnect;
    _onConnected=__onConnected;
    _onDisConnect=__onDisConnect;

    _onInvalid=__onInvalid;
    _onRefuse=__onRefus;

    _onSet=__onSet;
    _onAttack=__onAttack;
    _onChatIn=__onChatIn;

    try{
        server = new UDPServer(readData,standartPort);
    }
    catch(...){
        server = new UDPServer(readData,0);
    }
    port=(*server).getPort();

    srand(time(NULL));
    packegeID=rand();


    atimer=new timer(timeOut, TimeOutTime);
}

}

