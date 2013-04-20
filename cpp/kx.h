// kyuplex v08
#ifndef __KX_H_INCLUDED__
#define __KX_H_INCLUDED__

#include <irrlicht.h> // 3d engine and scene manager
#include <irrKlang.h> // sound lib
//#include <irrTypes.h>  // TODO: optimize #includes
//#include <Keycodes.h> etc...

// PHYSICS
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <iostream>

#ifdef _MSC_VER
#pragma comment (lib, "Irrlicht.lib")
#pragma comment (lib, "irrKlang.lib") 
//#pragma comment (linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;


#include "kxEnums.h"

// FORWARDS
class kxPlayerCube; 
class kxLevel; 
class kxSceneNodeFactory; 

// BULLET
/*
class btRigidBody;
class btQuaternion;
class btVector3;
class btAlignedObjectArray;
*/

//#define RED 255,200,0,0

/* vim style marker folding is used: 
 * vimdoc.sourceforge.net/htmldoc/fold.html#folding 
 */


struct kxKeyMap
{
    kxKeyMap () {};
    kxKeyMap( EKX_PLAYER_ACTION a, EKEY_CODE k, bool p) 
        : action( a ), keycode( k ), pressed( p ) {}

    EKX_PLAYER_ACTION action;
    EKEY_CODE keycode;
    bool pressed;
};


// GAME CLASS
class kx  //kx=kyuplex
{
    private:
        // SINGLETON DESIGN
    	kx() {};

        //kx(kx const&); // Don't Implement.
        //void operator=(kx const&);

    public:
        ~kx();

        // SINGLETON DESIGN
        static kx& getInstance() 
        {
            static kx instance;
            return instance;
        }

        // METHODS {{{

        // kxInit.cpp
        int Init();

        // kxGUI.cpp
        void InitGUI();
        void InitModal( const stringw&, const stringw& );
        void InitTextures();
        void DrawGUI() const;

        // kxSound.cpp
        void InitSounds();

        // kxBeginNewGame.cpp
        bool BeginNewGame();

        // kxGameSaveLoad.cpp
        bool SaveGame();
        bool LoadGame( const path& );
        // kxLevelSaveLoad.cpp
        void ResetScene();
        bool SaveLevel( EKX_LEVEL_FORMAT );
        bool LoadLevel();
        void LoadEntities();
        bool LoadBsp( const path& );
        //void LoadCollectables();
        //void LoadKillCubes();

        // kxHUD.cpp
        void DrawHUD() const;
        void DrawProgressBars() const;
        void DrawGrid() const;
        void DrawCollectableLines() const;
        void DrawAxii( const vector3df&, const f32 ) const;
        void DrawLights() const;

        // kxDebug.cpp
        void DrawDebugInfo() const;
        void LogDebugInfo() const;
        void VertexReport( IMesh* ) const;

        // kxExecute.cpp
		int Execute();

        // kxPlayerState.cpp
        void ProcessPlayerState();
        void ResetPlayer();
        // kxPlayerMovement.cpp
        void ProcessPlayerMovement();

        // kxEntities.cpp
        void ProcessEntities();

        // kxPhysics.cpp
        void ProcessPhysics();         
        void SyncTransform( ISceneNode*, const btTransform& );
        void ColorActivationState( btRigidBody* );
        void ClearPhysics();          
        void QuaternionToEuler( const btQuaternion&, btVector3& ); 
        vector3df Bt2irrVec3( const btVector3& );

        // kxLevel.cpp
        // kxProcessLevel.cpp
        void ProcessLevel();         
        void ProcessLevel1();         
        void ProcessLevel2();         
        void ProcessLevel3();         
        void ProcessLevel4();         
        void ProcessLevel5();         

        // end METHODS }}}


        // TIME 
        u32 fps, time, now, then, lastRenderTime, frame, numTicks; 
        f64 deltaTime;
 
        // DEBUG
        bool showDebugInfo, showPhysicsWireframes, showLights;
        IWriteFile* debugLog;
        bool logDebugInfo;

        array<kxKeyMap> keyMapArray;
        EKEY_CODE lastKey;  // for key event timed movement

        EKX_GAME_STATE gameState;
        u32 controlNodeID, controlType;
        ISceneNode* pControlNode;


        // IRRLICHT {{{
        IrrlichtDevice* device;

        IVideoDriver* driver;
        IGUIEnvironment* GUI; 
        ICursorControl* cursor;
        ISceneManager* smgr;
        IFileSystem* fileSys;

        path kxBaseDir;

        IEventReceiver* GUIEventReceiver;
        IEventReceiver* gameEventReceiver;
        IEventReceiver* editorEventReceiver;

        kxSceneNodeFactory* nodeFactory;

        // KX ENTITIES
        kxPlayerCube* playerCube;
        kxLevel* activeLevel;

        // NODES
        IMeshSceneNode* camPivot;
        vector3df camPivotRot;
        ICameraSceneNode* playerCam;

        f32 camDistance;

        stringw messages;
        u32 messagesNumLines;

        // end IRRLICHT }}}


        // IRR KLANG SOUND {{{
        ISoundEngine* sound; 

        ISoundSource* scaleUp;
        ISoundSource* scaleDown;

        ISoundSource* sineC3;
        ISoundSource* sineC3s;
        ISoundSource* sineD3;
        ISoundSource* sineE3;
        ISoundSource* sineF3;
        ISoundSource* sineG3;
        ISoundSource* sineA3;
        ISoundSource* sineB3;
        ISoundSource* sineC4;
        ISoundSource* sineD4;

        ISoundSource* triC1release;

        ISoundSource* triC1;
        ISoundSource* triD1;
        ISoundSource* triE1;
        ISoundSource* triF1;
        ISoundSource* triG1;
        ISoundSource* triA1;
        ISoundSource* triB1;
        ISoundSource* triC2;
        ISoundSource* triD2;
        ISoundSource* triE2;
        ISoundSource* triF2;
        ISoundSource* triG2;
        ISoundSource* triA2;
        ISoundSource* triB2;
        ISoundSource* triC3;
        ISoundSource* triD3;
        ISoundSource* triE3;
        ISoundSource* triF3;
        ISoundSource* triG3;
        ISoundSource* triA3;
        ISoundSource* triB3;
        ISoundSource* triC4;


        // end SOUND }}}


        // BULLET PHYSICS {{{

        btDefaultCollisionConfiguration* collisionConfiguration;
        btBroadphaseInterface* broadPhase;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;

        btDiscreteDynamicsWorld* physics;


        //u16 numCreatedObjects, numUpdatedBodies, numCons;
        //f32 physicsWorldScaling;
        //btVector3 worldGrav;  // gravity

        // shapes can be shared among multiple collision objects
        btAlignedObjectArray< btCollisionShape* > collisionShapes; // holds all shapes in the world

        //btAlignedObjectArray <btRigidBody*> shellRigidBodies; 
        //btAlignedObjectArray <btPoint2PointConstraint*> shellConstraints; 


        // RIGID BODIES
        //btRigidBody* testCubeRB;
        //btRigidBody* level01RigidBody;  

        // CONSTRAINTS
        //btGeneric6DofSpringConstraint* springCon1;
        //btPoint2PointConstraint* con;



        // end BULLET }}}


        // GUI {{{
        bool isGUIvisible, isFirstGUI;
        //bool quit;

        dimension2d< u32 > screenSize;
        position2di screenCenter, mousePos;
        f32 mouseSens;  // sensitivity
        f32 menuBorderMargin; // %
        u32 margin; // actual margin in pixels
        core::position2di winPos;  // upper left corner of gui windows
        u32 buttonHeight, buttonWidth, buttonSize;
        u32 numButtons;  // side dim, actual num of buttons = numButtons^2
        u32 titleBarHeight;

        
        IGUISkin* guiSkin;
        IGUIFont* largeFont; 
        IGUIFont* smallFont;

        IGUIElement* GUIroot;

        IGUIWindow* titleWindow;
            IGUIButton* beginButton;
            IGUIButton* settingsButton;
            IGUIButton* editorButton;
            IGUIButton* endButton;

        IGUIWindow* beginWindow;
            IGUIButton* beginNewButton;
            IGUIButton* beginLoadButton;
            IGUIButton* beginSaveButton;
            IGUIButton* beginBackButton;

        IGUIWindow* settingsWindow;
            IGUIButton* settingsPhysicsButton;
            IGUIButton* settingsBackButton;
            
        IGUIWindow* physicsWindow;
            IGUIEditBox* physicsEditGrav;
            IGUIButton* physicsBackButton;

        IGUIWindow* saveWindow;
            IGUIEditBox* saveEditBox;
            IGUIButton* saveOkButton;
        IGUIFileOpenDialog* fileOpenDialog; 
       
        IGUIStaticText* worldGravText;
        IGUISpinBox* worldGravXBox;
        IGUISpinBox* worldGravYBox;
        IGUISpinBox* worldGravZBox;
        IGUISpinBox* tCubeRestBox;

        // end GUI }}}


        // TEXTURES {{{ 
        ITexture* playerTex;
        ITexture* blackTex;
        ITexture* whiteTex;
        ITexture* grayTex;
        ITexture* darkGrayTex;
        ITexture* greenTex;
        ITexture* darkGreenTex;
        ITexture* neonGreenTex;
        ITexture* redTex;
        ITexture* blueTex;
        ITexture* purpleTex;
        ITexture* orangeTex;
        ITexture* yellowTex;
        ITexture* grayCheckers;
        ITexture* testCheckers;
        ITexture* lightTex;
        ITexture* soundToolTex;

        SMaterial playerMat;
        //SMaterial rigidMat;
        //SMaterial collectMat;
        //SMaterial killMat;
        // etc.. TODO:
        SMaterial whiteMat;
        SMaterial blackMat;
        // end TEXTURES }}}


        // (in Quake units, 0.1*20 = 2 meter)
};

#endif
