#include "Game/options.hpp"
#include "Network/BattleShipUDP.hpp"
#include "Graphics/Ogre.hpp"
#include "Game/Core.hpp"
#include "Music/Music.hpp"


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    BattleShipOptions::InitOptions();
    BattleShipUDP::initUdpProtocol(onFoundServer,onConnect,onConnected,onDisConnect,onInvalid,onRefuse,onSet,onAttack,onChatIn);
    BattleShipsMusic::Init();

    try
    {
        OgreStart();
    }
    catch( std::exception & e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.what();
#endif
    }

    return 0;
}
