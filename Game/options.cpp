#include <string>
using std::string;

namespace BattleShipOptions{

string Nick="Player",EnemyNick="Player";
unsigned ClientID, EnamiID;
bool Turn, NotSet=true;

void InitOptions(){
    srand(time(NULL));
    ClientID=rand();
}

}
