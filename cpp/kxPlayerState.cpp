
//#include "kx.h"
#include "kxPlayerCube.h"
#include "kxLevel.h"
#include "kxCollectable.h"
#include "kxCollectAnimator.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void kx::ProcessPlayerState()
{

    if( playerCube->state != EPS_DEAD && ( now - activeLevel->startTime > 100 )) 
    {
        // check if player impacts any object too hard
        btVector3 deltaV= playerCube->rb->getDeltaLinearVelocity();
        btScalar dSum= abs_( deltaV.x()) + abs_( deltaV.y()) + abs_( deltaV.z());

        // and is going fast
        // this avoids problems of fracturing when window focus is lost
        btVector3 vel= playerCube->rb->getLinearVelocity();
        btScalar vSum= abs_(vel.x()) + abs_(vel.y()) + abs_(vel.z());

        if( dSum * deltaTime > playerCube->impactDeathThreshold && vSum > playerCube->velDeathThreshold )
            playerCube->integrity--;

        if( playerCube->integrity <= 0 )
            playerCube->state= EPS_DEAD;
        else if( !playerCube->spring->isEnabled() )
            playerCube->state= EPS_DEAD;

        if( dSum * deltaTime > playerCube->impactSoundThreshold &&
            !sound->isCurrentlyPlaying( triC1 ) &&
            playerCube->state != EPS_DEAD)
                sound->play2D( triC1 );


        if( playerCube->state == EPS_DEAD )
        {
            playerCube->spring->setEnabled( false );
            //physics->removeConstraint (playerCube->spring);
            playerCube->Fracture( vector3df( 2.f ));
            sound->play2D( triC1release );
            return;
        }

       
        // collisions
        u32 n= playerCube->ghost->getNumOverlappingObjects();
        if( n && now - activeLevel->startTime > 1000 ) 
            for( u32 i=0; i<n; i++)
            {
                btCollisionObject* cObj= playerCube->ghost->getOverlappingObject( 0 );
                switch( cObj->getBroadphaseHandle()->m_collisionFilterGroup )
                {
                    case ECG_COLLECTABLE:
                    {
                        kxCollectable* c= static_cast<kxCollectable*>( cObj->getUserPointer() );

                        for (u32 i=0; i<activeLevel->collectables.size(); i++)
                            if (activeLevel->collectables[i] == c )
                                activeLevel->collectables.erase( i ); 
                        physics->removeCollisionObject( c->co );
                        c->remove();

                        kxCollectAnimator* collAnim = new kxCollectAnimator( now );
                        playerCube->addAnimator( collAnim );
                        collAnim->drop();

                        if( activeLevel->collectables.size() == 0 )
                            sound->play2D( sineG3 );
                        else 
                            sound->play2D( sineF3 );
                    } 
                    break;

                    case ECG_SOUND_TOOL:
                    {
                        std::cout<< "[[sound tool]] collected" <<std::endl;
                        ISceneNode* node= static_cast<ISceneNode*>( cObj->getUserPointer() );
                        node->remove();
                        physics->removeCollisionObject( cObj );

                        playerCube->hasSoundTool= true;
                        playerCube->activeTool= KT_SOUND;

                        // TODO: change this sound!
                        sound->play2D( sineG3 );

                        //kxCollectAnimator* collAnim = new kxCollectAnimator( now );
                        //playerCube->addAnimator( collAnim );
                        //collAnim->drop();

                        /*
                        if( activeLevel->collectables.size() == 0 )
                            sound->play2D( sineG3 );
                        else 
                            sound->play2D( sineF3 );
                            */
                    } 
                    break;

                    default: break;
                }
            }
    }

    if( playerCube->state == EPS_DEAD && !sound->isCurrentlyPlaying( triC1release ))
    {
        playerCube->reset();
        //playerCube= static_cast< kxPlayerCube* >( smgr->addSceneNode( "kxPlayerCube" ));
        //playerCube->setPosition( btVector3( 0,0.84f,0 ));
        //ResetPlayer ();
    }


        /*
        if (worldGrav.x()==0 && worldGrav.y()==0 && worldGrav.z()==0) 
        {
            // freeze frame effect
            // TODO: implement framerate independence here

            for (int i=0; i<playerCube->fractureArray.size(); i++)
            {
                btRigidBody* rb = playerCube->fractureArray[i];
                btVector3 vel = rb->getLinearVelocity();
                btScalar sum = abs_(vel.x()) + abs_(vel.y()) + abs_(vel.z());
                (sum<=4)? rb->setDamping (.75,.75) : rb->setDamping (1-1/sum,1-1/sum);
                ColorActivationState (rb);
            }
        }*/


    // SHELL CONSTRAINT STUFF
    /*
    numCons=0;
    for (int i=0; i<shellConstraints.size(); i++)
    {
        if (shellConstraints[i]->isEnabled()) 
            numCons++;
        else
        {
            ISceneNode* sn = static_cast<ISceneNode*> (shellConstraints[i]->getRigidBodyA().getUserPointer());
            if (sn->getID() != KNT_PLAYER_CUBE)
                sn->getMaterial(0).setTexture (0, purpleTex); 
            sn = static_cast<ISceneNode*> (shellConstraints[i]->getRigidBodyB().getUserPointer());
            if (sn->getID() != KNT_PLAYER_CUBE)
                sn->getMaterial(0).setTexture (0, purpleTex); 
        }
    }

    //playerIntegrity = shellConstraints.size()/2-numCons;
    playerIntegrity = numCons;
    */

    /*
    if (shellLost)
    {
        playerMoveSpeed=10;
        playerSpeedStep=10;
    }
        pCubeSpringCon->setEnabled (false);
        pHeartRB->setDamping (0,0);
        playerCube->state=EPS_DEAD;
        playerCube->getMaterial(0).setTexture (0, grayTex); 
        //playerIntegrity=0;
    }
    */

}

void kx::ResetPlayer()
{
    //playerCube->moveDir = vector3df (0,0,0);
    playerCube->rb->setLinearVelocity( btVector3(0,0,0));
    playerCube->rb->setAngularVelocity( btVector3(0,0,0));
    playerCube->rb->clearForces();
    playerCube->rb->updateInertiaTensor();
    playerCube->lastImpTime= now + 100;
    playerCube->setPosition( vector3df (0,.84f,0));
}
