#include "Ogre.hpp"
#include "CEGUI.hpp"
#include "../Game/options.hpp"
#include "../Game/Core.hpp"



    Ogre::Root *mRoot;
    Ogre::Camera *mCamera;
    Ogre::SceneManager *mSceneMgr;
    Ogre::RenderWindow *mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    //OIS Input devices
    OIS::InputManager *mInputManager;
    OIS::Mouse *mMouse;
    OIS::Keyboard *mKeyboard;

    bool mShutDown = false;


void setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}


bool configure(void)
{
    if(mRoot->showConfigDialog())
    {
        mWindow = mRoot->initialise(true, "BattleShips3D");
        return true;
    }

    return false;
}


void chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}



void createCamera(void)
{
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setPosition(Ogre::Vector3(0,0,80));
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
}



void createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}


void loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


void windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

TEvents Events;

void createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(&Events);
    mKeyboard->setEventCallback(&Events);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, &Events);

    mRoot->addFrameListener(&Events);
}

bool setup(){

    mRoot = new Ogre::Root(mPluginsCfg);
    setupResources();
    bool carryOn = configure();
    if (!carryOn)
        return false;
    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
   // createResourceListener();

    // Load resources
    loadResources();

    loadGUI();
    // Create the scene
    createScene();


    createFrameListener();

    return true;
}


void OgreStart(){
    mResourcesCfg = BattleShipOptions::OgreResourcesCfg;
    mPluginsCfg = BattleShipOptions::OgrePluginsCfg;

    if (!setup())
        return;

    mRoot->startRendering();

    destroyScene();
}



