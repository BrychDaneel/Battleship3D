#include "CEGUI.hpp"
#include "Ogre.hpp"
#include "../Game/Core.hpp"
#include <CEGUI/ScriptModules/Lua/ScriptModule.h>

CEGUI::Window *guiRoot;


bool TEvents::frameStarted (const Ogre::FrameEvent &evt){
    return true;
}

bool TEvents::frameEnded (const Ogre::FrameEvent &evt){
    for (int i; i<100000; i++);
    return true;
}

bool TEvents::frameRenderingQueued(const Ogre::FrameEvent& evt){
    if(mWindow->isClosed())
        return false;

    if (mShutDown)
        return false;

    mKeyboard->capture();
    mMouse->capture();

    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
    ToMainThread();
    return true;
}

bool TEvents::keyPressed( const OIS::KeyEvent &arg ){
    if (arg.key==OIS::KC_ESCAPE)
        mShutDown=true;

    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    return true;
}

bool TEvents::keyReleased( const OIS::KeyEvent &arg ){
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    return true;
}


CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}


bool TEvents::mouseMoved( const OIS::MouseEvent &arg ){
    CEGUI::GUIContext &sys = CEGUI::System::getSingleton().getDefaultGUIContext();
    sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);

    if (arg.state.Z.rel)
        sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

    OnMouseMove();
    return true;
}

bool TEvents::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    OnMouseClick();
    return true;
}

bool TEvents::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    return true;
}


void loadGUI(){
    CEGUI::OgreRenderer& myRenderer =   CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::LuaScriptModule::setDefaultResourceGroup("Lua_scripts");
    CEGUI::LuaScriptModule& scriptmod = CEGUI::LuaScriptModule::create();
    CEGUI::System::getSingleton().setScriptingModule(&scriptmod);
    CEGUI::System::getSingleton().executeScriptFile("BattleShips.lua");

    guiRoot = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

}





