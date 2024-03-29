#ifndef __KX_GAME_EVENTRECEIVER_H_INCLUDED__
#define __KX_GAME_EVENTRECEIVER_H_INCLUDED__

//#include "kx.h"
#include "kxPlayerCube.h" 
#include "kxRigidBox.h" 
#include "kxSoundSquareAnim.h" 
#include "kxSpline.h" 

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxGameEventReceiver : public IEventReceiver
{
public:
	kxGameEventReceiver( kx& context ) : k( context ) {}

	virtual bool OnEvent( const SEvent& event )
	{

        // RELEASED KEYS
		if( event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown ) 
		{
            switch( event.KeyInput.Key )
            {
            case KEY_ESCAPE:
                k.GUI->setFocus( k.titleWindow );
                k.isGUIvisible = true;
                //k.gameState= EGS_GUI;
                k.device->setEventReceiver( k.GUIEventReceiver );
                k.cursor->setPosition( k.screenCenter );
                return true;

            //case KEY_TAB:
                // TODO: show HUD menu


            // MANUAL PLAYERSTATE CHANGES
            /*
            case KEY_KEY_Q:
                if (k.playerCube->state != EPS_CUBE)
                {
                    k.playerCube->state = EPS_CUBE;
                    k.camPivot->setRotation (k.playerCube->getRotation());
                    k.playerCam->setParent (k.camPivot);
                    //k.playerCube->setRotation (vector3df (0,45,0));
                }
                return true;
            case KEY_KEY_E:
                if (k.playerCube->state != EPS_WEAPON)
                {
                    k.playerCube->state = EPS_WEAPON;
                    // TODO: 
                    //k.playerCube->setRotation (k.camPivot->getRotation());
                    //k.playerCam->setParent (k.playerCube->node);
                }
                return true;

            case KEY_KEY_R:
                (k.controlNodeID == KNT_PLAYER_CUBE)?
                    k.controlNodeID = KNT_TESTCUBE :
                    k.controlNodeID = KNT_PLAYER_CUBE;
                return true;
            */

            default: break;
            }
        } // END KEYBOARD


        /*
         * TODO: ECT_VELOCITY
         * this is control type velocity
		if (event.EventType == EET_KEY_INPUT_EVENT && !k.isGUIvisible) 
		{
            btVector3 linVel (0,0,0); //= k.pCubeRigidBody->getLinearVelocity ();
            switch(event.KeyInput.Key)
            {
            // MOVEMENT
            case KEY_KEY_J:
                (event.KeyInput.PressedDown)? 
                    linVel = btVector3 (k.playerMoveSpeed,0,0):
                    linVel = btVector3 (0,0,0); 
                k.pCubeRigidBody->setLinearVelocity (linVel);

                return true;

            //case KEY_KEY_G:
                //k.tCube9RigBod->setLinearVelocity (btVector3 (-10,0,0));
                //return true;

            default: break;
            }
        }
        */

        // MOVEMENT
        /* keyboard events occur on keyboard input timing, 
         * so for time-sensitive movement
         * we must use the keyMapArray to transport 
         * those events to the movement functions which occur every game tick
         * */
        if( event.EventType == EET_KEY_INPUT_EVENT ) 
		{
            for( u32 i=0; i<k.keyMapArray.size(); ++i )
            {
                if( k.keyMapArray[i].keycode == event.KeyInput.Key )
                {
                    k.keyMapArray[i].pressed= event.KeyInput.PressedDown;
                    return true;
                }
            }
        } // END MOVEMENT


        // MOUSE
        if( event.EventType == EET_MOUSE_INPUT_EVENT )
        {
            // MOUSE LOOK
            // TODO: implement framerate independence here
            if( event.MouseInput.Event == EMIE_MOUSE_MOVED )
            {
                k.mousePos= k.cursor->getPosition();
                if( k.mousePos != k.screenCenter )
                {
                    // LEFT/RIGHT ROTATION
                    f32 deltaX= f32( k.mousePos.X - k.screenCenter.X );
                    deltaX *= k.mouseSens;
                    f32 oldY= ( k.playerCube->state==EPS_WEAPON ) ?
                        k.playerCube->getRotation().Y : k.camPivot->getRotation().Y;
                    f32 newY= oldY+deltaX;

                    // UP/DOWN ROTATION
                    f32 deltaY= f32( k.mousePos.Y - k.screenCenter.Y );
                    deltaY *= k.mouseSens;
                    f32 oldX= ( k.playerCube->state == EPS_WEAPON ) ?
                        k.playerCube->getRotation().X : k.camPivot->getRotation().X;
                    f32 newX = oldX-deltaY;

                    // FIXME to avoid gimbal lock
                    if( newX > 89.f ) newX= 89.f;
                    else if( newX < -89.f ) newX= -89.f;

                    //k.playerCube->state == EPS_WEAPON ?
                        //k.playerCube->setRotation( vector3df( newX,newY,0 )):
                    k.camPivot->setRotation( vector3df( newX,newY,0 ));

                    k.cursor->setPosition( k.screenCenter );
                }
            } // END MOUSE LOOK

            // MOUSE CLICK
            if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
            {
                if( k.playerCube->hasSoundTool && k.playerCube->activeTool == KT_SOUND ) {
                    std::cout<< "spawn a [[sound]]" <<std::endl;

                    // sound_square
                    array< S3DVertex > vertices;
                    S3DVertex vx;
                    vx.Color= SColor(255,255,255,255); // what color?

                    vx.Pos= vector3df( -1.6f,0,1.6f );
                    vertices.push_back( vx );
                    vx.Pos= vector3df( 1.6f,0,1.6f );
                    vertices.push_back( vx );
                    vx.Pos= vector3df( 1.6f,0,-1.6f );
                    vertices.push_back( vx );
                    vx.Pos= vector3df( -1.6f,0,-1.6f );
                    vertices.push_back( vx );

                    SMaterial sMat; 
                    sMat.Lighting= false;
                    sMat.MaterialType= EMT_TRANSPARENT_ADD_COLOR;

                    kxSpline* sNode= new kxSpline( vertices, false, sMat );
                    sNode->setName( "sound_square");
                    sNode->setPosition( k.playerCube->getPosition() );
                    kxSoundSquareAnim* anim= new kxSoundSquareAnim( k.now );
                    sNode->addAnimator( anim );
                    anim->drop();

                    k.sound->play2D( k.sound_tool_emit );

                    //k.activeLevel->splineArray.push_back( sNode );

                    /*
                    kxRigidBox* rbx= new kxRigidBox( vector3df( 1.6f, 0.01f, 1.6f ));
                    rbx->setPosition( (const btVector3&) k.playerCube->getPosition() ); 
                    rbx->rb->getBroadphaseHandle()->m_collisionFilterGroup= ECG_SOUND_BOX;
                    rbx->setColor( SColor(0,0,0,0) );
                    rbx->setEdgeColor( SColor(255,255,255,255) );
                    rbx->m_material.MaterialType= EMT_TRANSPARENT_ADD_COLOR; 
                    
                    kxRigidBoxScaleAnim* anim= new kxRigidBoxScaleAnim( k.now );
                    rbx->addAnimator( anim );
                    anim->drop();
                    //ISceneNodeAnimator* deleteAnim= k.smgr->createDeleteAnimator( 1000 );
                    //rbx->addAnimator( deleteAnim );
                    //deleteAnim->drop(); 
                    //k.activeLevel->rboxes.push_back( rbx );
                    */
                }
                return true;
            }

            // MOUSE WHEEL
            if( event.MouseInput.Event == EMIE_MOUSE_WHEEL )
            {
                if( k.playerCube->moveSpeed + event.MouseInput.Wheel * k.playerCube->speedStep > 0.f )
                    k.playerCube->moveSpeed += event.MouseInput.Wheel * k.playerCube->speedStep; 
                return true;
            }

        } // END MOUSE

		return false;
	}
private:
    kx& k;
};


#endif
