#include "Application.h"
#include "../Network/BattleShipUDP.hpp"
#include "../Game/options.hpp"
#include <strstream>

using namespace BattleShipUDP;
using namespace Ogre;
using namespace OgreBites;

std::vector<std::string> ServerBuffer;

int  myarea[10][10], enemyarea[10][10];
int limit[] = {0, 4,3,2,1};
bool enemyset=false, myset=false , Game=false;
bool wantStep2=false,wantEnemySet=false;
int wantX=-1, wantY=-1;

void onFoundServer(const int id){
    ServerBuffer.push_back(ServerList[id].name);
}

void onConnect(){
}

void onConnected(){
    wantStep2=true;
//    nTrayMgr->showOkDialog("Connect",BattleShipOptions::EnemyNick+" has been connected to you.");
}

void onDisConnect(){
  //  nTrayMgr->showOkDialog("Disconnect",BattleShipOptions::EnemyNick+" left the game.");
}

void onInvalid(){
}

void onRefuse(){
}


void onSet(const bool battle[10][10]){
    for (int i=0; i<10; i++)
        for (int ii=0; ii<10; ii++)
            enemyarea[i][ii]=battle[i][ii];
    enemyset=true;
    wantEnemySet=true;
}

void onAttack(const unsigned char x, const unsigned char y){
    wantX=x;
    wantY=y;
}

void onChatIn(const string message){
}

//-------------------------------------------------------------------------------------
Application::Application(void)
{
    for (int i=0; i<10; i++)
        for (int ii=0; ii<10 ;ii++)
            myarea[i][ii]=enemyarea[i][ii]=0;
}
//-------------------------------------------------------------------------------------
Application::~Application(void)
{
}

//-------------------------------------------------------------------------------------
void Application::createScene(void)
{
    BattleShipOptions::InitOptions();
    initUdpProtocol(onFoundServer,onConnect,onConnected,onDisConnect,onInvalid,onRefuse,onSet,onAttack,onChatIn);
  //  OpenServer("TestServer");
    // create your scene here :)

        Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setDiffuseColour(.3, .3, .3);
    pointLight->setSpecularColour(.3, .3, .3);
    pointLight->setPosition(0,200,0);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
    mSceneMgr->setSkyBox(true, "CloudyNoonSkyBox");

                            Ogre::Plane oceanSurface;
      oceanSurface.normal = Ogre::Vector3::UNIT_Y;
      oceanSurface.d = 20;
      Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
         oceanSurface,
         1000, 1000, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

      Ogre::Entity* mOceanSurfaceEnt = mSceneMgr->createEntity( "OceanSurface", "OceanSurface" );
      mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mOceanSurfaceEnt);
      mOceanSurfaceEnt->setMaterialName("Ocean2_HLSL_GLSL");
}


void Application::createFrameListener(void){
    BaseApplication::createFrameListener();

    mTrayMgr->showCursor();

    OgreBites::SelectMenu *SelectMenu=mTrayMgr->createThickSelectMenu(OgreBites::TL_CENTER, "ServerMenu", "Chose Server",200,5);

    mTrayMgr->createButton(OgreBites::TL_CENTER, "UpdateButton", "Update",200);
    mTrayMgr->createButton(OgreBites::TL_CENTER, "OpenButton", "Create Server",200);
    mTrayMgr->createButton(OgreBites::TL_CENTER, "ConnectButton", "Connect",200);
}


int len=1;
bool rot=0;
bool Opened=false;

Ogre::SceneNode* Select;
Ogre::Real ofX=1, ofY=1;
Ogre::Entity* SelectEntity;


void Application::Step2(){

                mTrayMgr->destroyWidget("UpdateButton");
                mTrayMgr->destroyWidget("OpenButton");
                mTrayMgr->destroyWidget("ConnectButton");
                mTrayMgr->destroyWidget("ServerMenu");

                mTrayMgr->createButton(OgreBites::TL_LEFT, "Len1Button", "Len1",100);
                mTrayMgr->createButton(OgreBites::TL_LEFT, "Len2Button", "Len2",100);
                mTrayMgr->createButton(OgreBites::TL_LEFT, "Len3Button", "Len3",100);
                mTrayMgr->createButton(OgreBites::TL_LEFT, "Len4Button", "Len4",100);
                mTrayMgr->createSeparator(OgreBites::TL_LEFT, "Separator",100);
                mTrayMgr->createButton(OgreBites::TL_LEFT, "RotateButton", "Rotate",100);
                mTrayMgr->createButton(OgreBites::TL_BOTTOM, "SetButton", "Set",100);

                SelectEntity = mSceneMgr->createEntity("cube.mesh");
                Select = mSceneMgr->getRootSceneNode()->createChildSceneNode();
                Select->attachObject(SelectEntity);
                Select->setVisible(false);
                Select->setScale(0.05,0.05,0.05);

                mCamera->setPosition(0,170,-80);
                mCamera->lookAt(0,-50,0);

                for (int i=0; i<10; i++)
                    for (int ii=0; ii<10; ii++){
                           std::ostringstream s;
                           s<<"MyPlane"<<i<<ii;
                           string planeName=s.str();

                           std::ostringstream ss;
                           ss<<"MyPlaneEntity"<<i<<ii;
                           string planeNameEntity=ss.str();

                           Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);
                            Ogre::MeshManager::getSingleton().createPlane(planeName,
                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                8,8,5,5,true,1,5,5,
                        Ogre::Vector3::UNIT_Z);


                            Ogre::Entity* planeEnt = mSceneMgr->createEntity(planeNameEntity, planeName);
                            planeEnt->setMaterialName("panel/gray");

                            Ogre::SceneNode* planeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
                            planeNode->attachObject(planeEnt);
                            planeNode->translate(i*10+20,10,ii*10-50);
                    }

                    for (int i=0; i<10; i++)
                    for (int ii=0; ii<10; ii++){
                           std::ostringstream s;
                           s<<"EnamyPlane"<<i<<ii;
                           string planeName=s.str();

                           std::ostringstream ss;
                           ss<<"EnamyPlaneEntity"<<i<<ii;
                           string planeNameEntity=ss.str();

                           Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);
                            Ogre::MeshManager::getSingleton().createPlane(planeName,
                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
                                8,8,5,5,true,1,5,5,
                        Ogre::Vector3::UNIT_Z);


                            Ogre::Entity* planeEnt = mSceneMgr->createEntity(planeNameEntity, planeName);
                            planeEnt->setMaterialName("panel/gray");

                            Ogre::SceneNode* planeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
                            planeNode->attachObject(planeEnt);
                            planeNode->translate(i*10-120,10,ii*10-50);
                    }

}



void Application::buttonHit(OgreBites::Button *button){
    if (button->getName()=="UpdateButton"){
           ((SelectMenu*)mTrayMgr->getWidget("ServerMenu"))->clearItems();
            Search();
    }

    if (button->getName()=="OpenButton"){
                    if (Opened){
                        CloseServer();
                        button->setCaption("Create Sever");
                    }
                    else{
                        OpenServer("TestServer");
                        button->setCaption("Close Sever");
                    }
                    Opened^=1;
    }

    if (button->getName()=="ConnectButton" && ((SelectMenu*)mTrayMgr->getWidget("ServerMenu"))->getNumItems()>0){
                Connect(((SelectMenu*)mTrayMgr->getWidget("ServerMenu"))->getSelectionIndex());
                BattleShipOptions::Turn=false;

                Step2();
                return;
    }

    if (button->getName()=="Len1Button"){
            len=1;
            Select->setScale(0.05,0.05,0.05);
            ofX=1;
            ofY=1;
        }
    if (button->getName()=="Len2Button"){
            len=2;
            Select->setScale(0.05,0.05,0.13);
            ofX=1;
            ofY=7;
            if (rot)
                Select->yaw(Radian(3.14/2));
            rot=false;
        }
    if (button->getName()=="Len3Button"){
            len=3;
            Select->setScale(0.05,0.05,0.20);
            ofX=1;
            ofY=13;
            if (rot)
                Select->yaw(Radian(3.14/2));
            rot=false;
        }
    if (button->getName()=="Len4Button"){
            len=4;
            Select->setScale(0.05,0.05,0.3);
            ofX=1;
            ofY=17;
            if (rot)
                Select->yaw(Radian(3.14/2));
            rot=false;
        }

    if (button->getName()=="RotateButton"){
        rot^=1;
            Ogre::Real w=ofX;
            ofX=ofY;
            ofY=w;
            if (rot)
                Select->yaw(Radian(3.14/2));
            else
                Select->yaw(Radian(-3.14/2));
        }

    if (button->getName()=="SetButton"){
        for (int i=0; i<4; i++)
            if (limit[i])
                return;

        bool a[10][10];
        for (int i=0; i<10; i++)
            for (int ii=0; ii<10; ii++)
                a[i][ii]=myarea[i][ii]>0;
        SetBattle(a);
        myset=true;

        mTrayMgr->destroyWidget("RotateButton");
        mTrayMgr->destroyWidget("SetButton");
        mTrayMgr->destroyWidget("Separator");
        mTrayMgr->destroyWidget("Len1Button");
        mTrayMgr->destroyWidget("Len2Button");
        mTrayMgr->destroyWidget("Len3Button");
        mTrayMgr->destroyWidget("Len4Button");

        if (myset && enemyset){
            mTrayMgr->showOkDialog("Start","GameStarted!");
            Game=true;
            }
        return;
    }


}


bool Application::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
  bool ret = BaseApplication::frameRenderingQueued(fe);

  while (ServerBuffer.size()>0){
        SelectMenu *m= (SelectMenu *) mTrayMgr->getWidget("ServerMenu");
        if (!m)
            break;
        m->addItem(ServerBuffer[0]);
        ServerBuffer.erase(ServerBuffer.begin());
    }

   if (wantStep2){
        wantStep2=false;
        Step2();
   }

   if (wantEnemySet){
    wantEnemySet=false;
    if (myset && enemyset){
            mTrayMgr->showOkDialog("Start","GameStarted!");
            Game=true;
        }
    else
        mTrayMgr->showOkDialog("Set","Protivnik gotov!");
   }

   if (wantX!=-1){
        std::ostringstream s;
        s<<"MyPlaneEntity"<<wantX<<wantY;
        Entity * panel=mSceneMgr->getEntity(s.str());
        if (myarea[wantX][wantY]){
            panel->setMaterialName("panel/red");
            myarea[wantX][wantY]=2;
            }
        else{
            panel->setMaterialName("panel/green");
            BattleShipOptions::Turn=true;
            myarea[wantX][wantY]=3;
        }
        wantX=wantY=-1;
        bool ans=true;
        for (int i=0; i<10; i++)
            for (int ii=0; ii<10; ii++)
                ans=ans && !(myarea[i][ii]==1);
        if (ans)
                mTrayMgr->showOkDialog("Loser","You are loser!");
   }

  return ret;
}



void tryMove(int x,int y){

            if (!(limit[len]>0))
            return;

        bool ans=true;
        for (int i=0; i<len; i++){
            int qx=x;
            int qy=y;
            if (rot)
                qx-=i;
            else
                qy-=i;

            ans=ans && qx<10 && qx>=0 && qy<10 && qy>=0;

            for (int ii=-1; ii<2; ii++)
                for (int iii=-1; iii<2; iii++){
                    int cx=qx+ii;
                    int cy=qy+iii;
                    if (cx<10 && cx>=0 && cy<10 && cy>=0)
                        ans=ans && !myarea[cx][cy];
                }
        }

        if (ans){
            Select->setVisible(true);
            Select->setPosition(x*10+20-ofX,5,y*10-50-ofY);
        }
}


int lastMyX=-1, lastMyY=-1;
int lastEnamyX=-1, lastEnamyY=-1;
bool Application::mouseMoved( const OIS::MouseEvent &arg ){
    bool ret=BaseApplication::mouseMoved(arg);


    float mouseX=mMouse->getMouseState().X.abs;
    float mouseY=mMouse->getMouseState().Y.abs;
    float screenX=mWindow->getWidth();
    float screenY=mWindow->getHeight();

    float offsetX = float(mouseX) / screenX;
    float offsetY = float(mouseY) / screenY;

    if (lastMyX!=-1){
        std::ostringstream s;
        s<<"MyPlaneEntity"<<lastMyX<<lastMyY;

        mSceneMgr->getEntity(s.str())->setMaterialName("panel/gray");
        lastMyX=lastMyY=-1;
        Select->setVisible(false);
    }

    if (lastEnamyX!=-1){
        if (enemyarea[lastEnamyX][lastEnamyY]!=2 && enemyarea[lastEnamyX][lastEnamyY]!=3){
            std::ostringstream s;
            s<<"EnamyPlaneEntity"<<lastEnamyX<<lastEnamyY;
            mSceneMgr->getEntity(s.str())->setMaterialName("panel/gray");
        }

        lastEnamyX=lastEnamyY=-1;
    }

    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(offsetX, offsetY);
    Ogre::RaySceneQuery* raySceneQuery = mSceneMgr->createRayQuery(mouseRay);
    Ogre::RaySceneQueryResult rayQuery = raySceneQuery->execute();



    for(Ogre::RaySceneQueryResult::iterator it = rayQuery.begin(); it != rayQuery.end(); ++it)
    {
        Ogre::MovableObject *object = it->movable; // Объект, с которым пересекается луч... проверь, что он принадлежит роботу
        string name=object->getName();
        if (!myset && name.find("MyPlaneEntity")!=std::string::npos && !myarea[name[13]-'0'][name[14]-'0']){
            Ogre::Entity * ent = mSceneMgr->getEntity(name);
            ent->setMaterialName("panel/blue");
            lastMyX=name[13]-'0';
            lastMyY=name[14]-'0';
            tryMove(lastMyX,lastMyY);
            }

        if (Game && BattleShipOptions::Turn && name.find("EnamyPlaneEntity")!=std::string::npos && enemyarea[name[16]-'0'][name[17]-'0']!=2 && enemyarea[name[16]-'0'][name[17]-'0']!=3){
                Ogre::Entity * ent = mSceneMgr->getEntity(name);
                ent->setMaterialName("panel/blue");
                lastEnamyX=name[16]-'0';
                lastEnamyY=name[17]-'0';
        }
    }


    return ret;
}



void trySet(int x,int y, SceneManager* mSceneMgr){
        if (!(limit[len]>0))
            return;

        bool ans=true;
        for (int i=0; i<len; i++){
            int qx=x;
            int qy=y;
            if (rot)
                qx-=i;
            else
                qy-=i;

            ans=ans && qx<10 && qx>=0 && qy<10 && qy>=0;

            for (int ii=-1; ii<2; ii++)
                for (int iii=-1; iii<2; iii++){
                    int cx=qx+ii;
                    int cy=qy+iii;
                    if (cx<10 && cx>=0 && cy<10 && cy>=0)
                        ans=ans && !myarea[cx][cy];
                }
        }

        if (ans){
            for (int i=0; i<len; i++){
            int qx=x;
            int qy=y;\
            if (rot)
                qx-=i;
            else
                qy-=i;
            myarea[qx][qy]=1;
            }

                Select->setVisible(true);
                Select->setPosition(x*10+20-ofX,5,y*10-50-ofY);
                limit[len]--;

                SelectEntity = mSceneMgr->createEntity("cube.mesh");
                Select = mSceneMgr->getRootSceneNode()->createChildSceneNode();
                Select->attachObject(SelectEntity);
                Select->setVisible(false);
                Select->setScale(0.05,0.05,0.05);
                ofX=1;
                ofY=1;
                len=1;
                rot=0;
        }
}


void Attack(int x,int y, SdkTrayManager* mTrayMgr, Entity * panel){
    Attack(x,y);
    bool p=false;
    if (enemyarea[x][y]){
        panel->setMaterialName("panel/red");
        p=true;
        enemyarea[x][y]=2;
        }
    else{
        panel->setMaterialName("panel/green");
        BattleShipOptions::Turn=false;
        enemyarea[wantX][wantY]=3;
    }


    bool ans=true;
        for (int i=0; i<10; i++)
            for (int ii=0; ii<10; ii++)
                ans=ans && !(enemyarea[i][ii]==1);
        if (ans){
                mTrayMgr->showOkDialog("Win","You are Winer!");
                p=false;
            }

    if (p){
        bool check=true;
        int ohx[4], ohy[4], un=0, uk=0;
        bool fl[10][10];
        for (int i=0; i<10; i++)
            for (int ii=0; ii<10; ii++)
                fl[i][ii]=0;
        ohx[0]=x;
        ohy[0]=y;
        while (un<=uk){
                if (enemyarea[ohx[un]][ohy[un]]==1){
                    check=false;
                    break;
                }
                if (ohx[un]+1<10 && !fl[ohx[un]+1][ohy[un]] && enemyarea[ohx[un]+1][ohy[un]]==2 ||  enemyarea[ohx[un]+1][ohy[un]]==1 ){
                    ohx[++uk]=ohx[un]+1;
                    ohy[uk]=ohy[un];
                    fl[ohx[un]+1][ohy[un]]=1;
                }

                if (ohy[un]+1<10 && !fl[ohx[un]][ohy[un]+1] && enemyarea[ohx[un]][ohy[un]+1]==2  ||  enemyarea[ohx[un]][ohy[un]+1]==1){
                    ohx[++uk]=ohx[un];
                    ohy[uk]=ohy[un]+1;
                    fl[ohx[un]][ohy[un]+1]=1;
                }

                if (ohx[un]-1>=0 && !fl[ohx[un]-1][ohy[un]] && enemyarea[ohx[un]-1][ohy[un]]==2  ||  enemyarea[ohx[un]-1][ohy[un]]==1 ){
                    ohx[++uk]=ohx[un]-1;
                    ohy[uk]=ohy[un];
                    fl[ohx[un]-1][ohy[un]]=1;
                }

                if (ohy[un]-1>=0 && !fl[ohx[un]][ohy[un]-1] && enemyarea[ohx[un]][ohy[un]-1]==2  ||  enemyarea[ohx[un]][ohy[un]-1]==1){
                    ohx[++uk]=ohx[un];
                    ohy[uk]=ohy[un]-1;
                    fl[ohx[un]][ohy[un]-1]=1;
                }

                un++;
        }

        if (check)
            mTrayMgr->showOkDialog("Killer","Kill!");

    }
}


bool Application::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
    bool ret=BaseApplication::mousePressed(arg,id);

    float mouseX=mMouse->getMouseState().X.abs;
    float mouseY=mMouse->getMouseState().Y.abs;
    float screenX=mWindow->getWidth();
    float screenY=mWindow->getHeight();

    float offsetX = float(mouseX) / screenX;
    float offsetY = float(mouseY) / screenY;
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(offsetX, offsetY);
    Ogre::RaySceneQuery* raySceneQuery = mSceneMgr->createRayQuery(mouseRay);
    Ogre::RaySceneQueryResult rayQuery = raySceneQuery->execute();



    for(Ogre::RaySceneQueryResult::iterator it = rayQuery.begin(); it != rayQuery.end(); ++it)
    {
        Ogre::MovableObject *object = it->movable; // Объект, с которым пересекается луч... проверь, что он принадлежит роботу
        string name=object->getName();
        if (name.find("MyPlaneEntity")!=std::string::npos && !myarea[name[13]-'0'][name[14]-'0']){
            if (!myset){
                trySet(name[13]-'0',name[14]-'0',mSceneMgr);
            }
            }

        if (Game && BattleShipOptions::Turn && name.find("EnamyPlaneEntity")!=std::string::npos && enemyarea[name[16]-'0'][name[17]-'0']!=2 && enemyarea[name[16]-'0'][name[17]-'0']!=3){
                Attack(name[16]-'0',name[17]-'0',mTrayMgr, mSceneMgr->getEntity(name));
            }
    }


    return ret;
}

