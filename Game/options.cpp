#include <string>
#include <stdlib.h>
using std::string;

namespace BattleShipOptions{

string Nick="Player",EnemyNick="Player";
unsigned ClientID, EnamiID;
bool Turn=true, NotSet=true;

string MasterIP="127.0.0.1", MasterHost="127.0.0.1";
unsigned short MasterPort=80;
string Page_List="/BattleShips/ServerList.php";
string Page_Add="/BattleShips/AddServer.php";
string Page_Connect="/BattleShips/Connect.php";
string Page_Disconnect="/BattleShips/Disconnect.php";
string Page_Set="/BattleShips/Set.php";
string Page_Attack="/BattleShips/Attack.php";
string Page_Say="/BattleShips/Say.php";

void InitOptions(){
    srand(time(NULL));
    ClientID=rand();
}

}
