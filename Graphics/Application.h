#ifndef __Application_h_
#define __Application_h_

#include "BaseApplication.h"
#include <string>
using std::string;

class Application : public BaseApplication
{
public:
    Application(void);
    virtual ~Application(void);

protected:
    virtual void createScene(void);
    virtual void buttonHit(OgreBites::Button *button);
    virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    void Step2();
};

#endif
