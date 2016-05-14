#include "Network/BattleShipUDP.hpp"
#include <iostream>
#include "Game/options.hpp"

using namespace BattleShipUDP;
using namespace std;

void onFoundServer(const int id){
    cout<<"Found Server: quick="<<(ServerList[id].quick?1:0)<<" name="<<ServerList[id].name<<" ip="<<ServerList[id].ip<<" port="<<ServerList[id].port<<" fav="<<int(ServerList[id].fav)<<endl;
}

void onConnect(){
    cout<<"You are ssucsessfuly connected"<<endl;
}

void onConnected(){
        cout<<"Player name="<<BattleShipOptions::EnemyNick<<" has been connected to you"<<endl;
}

void onDisConnect(){
    cout<<"disconnect"<<endl;
}

void onInvalid(){
    cout<<"Invalid password"<<endl;
}

void onRefuse(){
    cout<<"refuse"<<endl;
}


void onSet(const bool battle[10][10]){
        cout<<"enami battle is ";
            for (int i=0; i<10; i++)
                for (int ii=0; ii<10; ii++)
                    cout<<battle[i][ii]?1:0;
        cout<<endl;
}

void onAttack(const unsigned char x, const unsigned char y){
    cout<<"Enami Attack You at "<<(int)x<<":"<<(int)y<<endl;
}

void onChatIn(const string message){
    cout<<"Enami says to you:"<<endl;
    cout<<message<<endl;
}

bool a[10][10];
int main(){

    BattleShipOptions::InitOptions();
    initUdpProtocol(onFoundServer,onConnect,onConnected,onDisConnect,onInvalid,onRefuse,onSet,onAttack,onChatIn);

    cout<<"You port is "<<port<<endl;
    cout<<"Print name of you server:"<<endl;
    string s;
    getline(cin, s);
    BattleShipUDP::OpenServer(s,"pass123");

    cout<<"Dont press anymore at first programm, press somethink to seach for servers:"<<endl;
    getline(cin, s);
    //AddFavorite("192.168.100.31",56021);
    BattleShipUDP::Search();

    cout<<"Enter somethink to connect with wrong pass to first founded server:"<<endl;
    getline(cin, s);
    BattleShipUDP::Connect(0,"pass125");

    cout<<"Enter somethink to connect with right pass to first founded server:"<<endl;
    getline(cin, s);
    BattleShipUDP::Connect(0,"pass123");

    cout<<"Enter somethink to say:"<<endl;
    getline(cin, s);
    BattleShipUDP::Say(s);
    BattleShipUDP::Say("Its packege for speedtest");

    cout<<"Enter somethink to set battle area:"<<endl;
    getline(cin, s);
    a[0][0]=1;
    a[0][1]=1;
    BattleShipUDP::SetBattle(a);

    cout<<"Enter two int to attack {x} {y}:"<<endl;
    unsigned int x,y;
    cin>>x>>y;
    getline(cin, s);
    BattleShipUDP::Attack(x,y);

    cout<<"Its all."<<endl;
    getline(cin, s);
}
