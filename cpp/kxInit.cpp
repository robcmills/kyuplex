
#include "kxSceneNodeFactory.h"
#include "kxLevel.h"
#include "kxPlayerCube.h"
#include "kxCollectable.h"
#include "kxDebugDrawer.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;

// defined in kxPhysics.cpp
extern void TickCallback( btDynamicsWorld*, btScalar ); // use of extern is optional

int kx::Init()
{
    fps=0;
    now=0;
    then=0;
    lastRenderTime=0;
    frame=0;
    numTicks=0;
    deltaTime=0;

    logDebugInfo= false;

    pControlNode=0;
    controlNodeID=0;
    controlType=0; 
    //lastKey = 0;

    // TODO: move to playerCube
    camDistance=0;
    messages=L" ";
    messagesNumLines=0;

    // PHYSICS
    //numUpdatedBodies=0;
    //numCreatedObjects=0;
    //numCons=0; 
    //physicsWorldScaling= 1.f;
    //worldGrav=btVector3( 0,0,0 );

    gameState= EGS_INIT;

    // DEBUG
    showDebugInfo= true;
    showPhysicsWireframes= true;
    showLights= true;

    // GUI / HUD
    isGUIvisible= true;
    isFirstGUI= true;
    mousePos.X=0;
    mousePos.Y=0;
    mouseSens= .1f;
    menuBorderMargin= 0.1f;
    titleBarHeight= 19;


    // DEFAULT KEYMAP
	keyMapArray.push_back( kxKeyMap( EPA_MOVE_FORWARD, KEY_KEY_W, 0 ));
    keyMapArray.push_back( kxKeyMap( EPA_MOVE_BACKWARD, KEY_KEY_S, 0 ));
    keyMapArray.push_back( kxKeyMap( EPA_MOVE_LEFT, KEY_KEY_A, 0 ));
    keyMapArray.push_back( kxKeyMap( EPA_MOVE_RIGHT, KEY_KEY_D, 0 ));
    keyMapArray.push_back( kxKeyMap( EPA_MOVE_UP, KEY_SPACE, 0 ));
    keyMapArray.push_back( kxKeyMap( EPA_MOVE_DOWN, KEY_LSHIFT, 0 ));


    // CREATE 3D ENGINE
    // IRRLICHT DEVICE
    device=0;
	device = createDevice 
    (
        video::EDT_OPENGL,
		core::dimension2d< u32 >( 1280, 800 ),
        32,
        false, // fullscreen
        true, // stencil buffer
        false  // vsync
    );
	if (device==0)
    {
        std::cout<<"Could not create device" << std::endl; 
		return 1; 
    }
	device->setWindowCaption( L"kyuplex" );

	driver= device->getVideoDriver();
    smgr= device->getSceneManager(); 
    smgr->getRootSceneNode()->setID( KNT_ROOT_NODE );
    smgr->getRootSceneNode()->setName( "kxRootSceneNode" );

    nodeFactory= new kxSceneNodeFactory( smgr ); 
    smgr->registerSceneNodeFactory( nodeFactory );

    fileSys= smgr->getFileSystem();
    kxBaseDir= fileSys->getWorkingDirectory();

    cursor= device->getCursorControl();

    screenSize= driver->getScreenSize();
    screenCenter= position2di( screenSize.Width/2, screenSize.Height/2 );
    
    InitTextures();
    InitGUI();


    // IRRKLANG SOUND
    sound=0;
    sound= createIrrKlangDevice();
    if (sound==0)
    {
        std::cout<< "Could not create sound device" << std::endl;
        return 1;  
    }

    InitSounds();


	// BULLET PHYSICS
    physics=0;
	collisionConfiguration= new btDefaultCollisionConfiguration();
	broadPhase= new btAxisSweep3( btVector3( -64, -64, -64), btVector3( 64, 64, 64 ));
	dispatcher= new btCollisionDispatcher( collisionConfiguration );
	solver= new btSequentialImpulseConstraintSolver();
	physics= new btDiscreteDynamicsWorld( dispatcher, broadPhase, solver, collisionConfiguration );

    if (physics==0)
    {
        std::cout<< "Could not create physics world" << std::endl;
        return 1;  
    }

    //physics->setInternalTickCallback( TickCallback );
	broadPhase->getOverlappingPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );

    // DEBUG DRAWER
	physics->setDebugDrawer( new kxDebugDrawer( driver ));

    // DEBUG LOG
    debugLog=0;
    debugLog = device->getFileSystem()->createAndWriteFile( "./debugLog.txt", true );
    //debugLog->write ("test", 4);
    if (debugLog==0)
        std::cout<< "Could not create debug log" << std::endl;

    return 0;
}

kx::~kx()
{
    //smgr->clear();  // why does this throw an error?
    ClearPhysics();

    // delete in opposite order of init
	delete physics;

	delete solver;
	delete dispatcher;
	delete broadPhase;
	delete collisionConfiguration;
}
