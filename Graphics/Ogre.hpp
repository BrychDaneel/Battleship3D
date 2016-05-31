#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreParticleSystem.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreMeshManager.h>

#include <OgreWindowEventUtilities.h>


void OgreStart();

class TEvents: public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener{
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameStarted (const Ogre::FrameEvent &evt);
    virtual bool frameEnded (const Ogre::FrameEvent &evt);

    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
};

extern bool mShutDown;


extern    Ogre::Root *mRoot;
extern    Ogre::Camera *mCamera;
extern    Ogre::SceneManager *mSceneMgr;
extern    Ogre::RenderWindow *mWindow;


    //OIS Input devices
extern    OIS::InputManager *mInputManager;
extern    OIS::Mouse *mMouse;
extern    OIS::Keyboard *mKeyboard;
