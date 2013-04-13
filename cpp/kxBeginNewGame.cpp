
#include "kx.h"
#include "kxPlayerCube.h"
//#include "kxQuadPlane.h"
//#include "kxCollectable.h"
#include "kxLevel.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


bool kx::BeginNewGame()
{
    playerCube= new kxPlayerCube();
    //if( !playerCube ) return false;
    activeLevel= new kxLevel();
    //if( !activeLevel ) return false;

    // camPivot
    // Acts as pivot point parent for cam
    //camPivot = smgr->addEmptySceneNode( smgr->getRootSceneNode(), KNT_CAM_PIVOT );
    camPivot = smgr->addCubeSceneNode( .8f, playerCube, KNT_CAM_PIVOT );
    camPivot->setName( "camPivot" );
    camPivot->setPosition( playerCube->getPosition() );
    camPivot->getMesh()->getMeshBuffer(0)->getMaterial().setTexture( 0, grayTex );
	camPivot->getMesh()->getMeshBuffer(0)->getMaterial().Lighting= false;
	camPivot->getMesh()->getMeshBuffer(0)->getMaterial().MaterialType= EMT_TRANSPARENT_ADD_COLOR;

    // camPivotChild
    /*
    camPivotChild = smgr->addEmptySceneNode( camPivot, KNT_CAM_PIVOT_CHILD );
    camPivotChild->setName( "camPivotChild" );
    camPivotChild->setPosition( playerCube->getPosition() );
    */

    // playerCamera
    camDistance= 12.8f;
    playerCam= smgr->addCameraSceneNode( camPivot, vector3df( 0,0,camDistance ), camPivot->getPosition(), KNT_CAMERA );
    playerCam->setName( "playerCamera" );
    playerCam->setFarValue( 10000.f );
    //camPivotRot= vector3df( -9.6f,45.f,0 );
    camPivot->setRotation( vector3df( -9.6f,45.f,0 ));
    //playerCam->setFOV (100); 

    //showPhysicsWireframes= false;
    controlType = ECT_IMPULSE; 
    controlNodeID = KNT_PLAYER_CUBE; // TODO: change this to unique node name
    pControlNode= playerCube;

    physics->setGravity( btVector3( 0,0,0 ));

    if( !LoadLevel() )
    {
        messages= "";
        messages += L"Error: could not load level 1";
        messagesNumLines= 1;
        return false;
    }

    gameState= EGS_GAME;
    isGUIvisible= false; 
    isFirstGUI= false;
    cursor->setPosition( screenCenter );
    device->setEventReceiver( gameEventReceiver );
    return true;
}
