
//#include "kx.h"
#include "kxCollectable.h"
#include "kxPlayerCube.h"
#include "kxFractureCube.h"
#include "kxLevel.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// not using yet...
// using btGhostObject <<
void TickCallback( btDynamicsWorld* w, btScalar timeStep )
{
    kx& k = kx::getInstance(); // context


    // Do work with contacts...
    /*
	int numManifolds = k.physics->getDispatcher()->getNumManifolds();
	for (int i=0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold =  k.physics->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*> (contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*> (contactManifold->getBody1());


        kxCube* cubeA=0;
        kxCube* cubeB=0;
        cubeA = (kxCube*) obA->getUserPointer();
        cubeB = (kxCube*) obB->getUserPointer();
        if (cubeA && cubeB)
        { 
            // collectable contacted
            if ((cubeA->getID() == KNT_PLAYER_CUBE && cubeB->getID() == KNT_COLLECTABLE) ||
                (cubeB->getID() == KNT_PLAYER_CUBE && cubeA->getID() == KNT_COLLECTABLE)) 
            {
                cubeA->getID()==KNT_PLAYER_CUBE ? 
                    static_cast<kxCollectable*>(cubeB)->Collected() : 
                    static_cast<kxCollectable*>(cubeA)->Collected(); 
            }
        }

		int numContacts = contactManifold->getNumContacts();
		for (int j=0; j<numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}
    }
    */
}


void kx::ProcessPhysics()
{

    //numTicks=0; // if logDebugInfo uncomment
    
	physics->stepSimulation( (btScalar) deltaTime, 10, .001f ); 

    if( logDebugInfo )
    {
        stringc log;
        log += "numticks";
        log += numTicks;
        debugLog->write( log.c_str(), log.size() );
        numTicks=0;
    }

    //ColorActivationState (tCube1RigidBody);

    // color edges depending on rigid body state
    for( u32 i=0; i<activeLevel->fractureCubes.size(); i++ )
    {
        kxFractureCube* fc= activeLevel->fractureCubes[i];
        const int aState= fc->rb->getActivationState();
        SColor color;
        switch( aState ) {
            // state?=purple
            case 0: color= SColor(255,175,0,255); break;
            // active=white
            case 1: color= SColor(255,255,255,255); break;
            // sleeping=darkGray
            case 2: color= SColor(255,60,60,60); break;
            // wantsActive=yellow
            case 3: color= SColor(255,255,200,0); break;
            default: color= SColor(255,255,255,255); break;
        }
        if( fc->m_edgeColor != color ) 
            fc->setEdgeColor( color );
    }


    btTransform xform= playerCube->rb->getWorldTransform();
    // update pivot point of playerCube spring constraint
    playerCube->spring->getFrameOffsetA().setOrigin( xform.getOrigin() ); //((const btVector3&) newPos); 
    // sync ghost 
    playerCube->ghost->setWorldTransform( xform );

    vector3df pos= Bt2irrVec3( xform.getOrigin() );
    //vector3df pos = playerCube->getPosition();

    if( controlNodeID == KNT_PLAYER_CUBE )
    {
        //camPivot->setPosition( pos );
        playerCam->setTarget( pos );

        //vector3df playerRot= playerCube->getRotation();
        //playerRot /= vector3df( 2.f );
        //playerRot.X *= -1.f;
        //camPivot->setRotation( camPivotRot + playerRot );
    }

    else if( controlNodeID == KNT_TESTCUBE )
    {
        /*
        xform = tCube1RigidBody->getWorldTransform ();
        pos = Bt2irrVec3 (xform.getOrigin());
        pos += vector3df (-.8,.8,.8);  // adjust to center of 8 cubes
        camPivot->setPosition (pos);
        playerCam->setTarget (pos);
        */
    }

}

// deprecated! using motionStates...
// synchronizes pos and rot between btRigidBody and irr::scene::ISceneNode
// used instead of btMotionState
void kx::SyncTransform( ISceneNode* node, const btTransform& t )
{
    // newPos
    const btVector3& point = t.getOrigin();
    vector3df newPos( vector3df( (f32)point[0], (f32)point[1], (f32)point[2] )); /// k->physicsWorldScaling);

    // newRot
    btVector3 euler;
    QuaternionToEuler( t.getRotation(), euler );
    vector3df newRot( vector3df (euler[0], euler[1], euler[2] ));

    node->setPosition( newPos );
    node->setRotation( newRot );
}


// used for debugging
void kx::ColorActivationState( btRigidBody* rb )
{
    ISceneNode* sn = static_cast<ISceneNode*>( rb->getUserPointer() );
    int aState = rb->getActivationState();

    if( aState == 0 ) sn->getMaterial(0).setTexture( 0, grayTex ); 
    else if( aState == 1 ) sn->getMaterial(0).setTexture( 0, purpleTex ); 
    else if( aState == 2 ) sn->getMaterial(0).setTexture( 0, blueTex ); 
    //else if (aState == 4) sn->getMaterial(0).setTexture (0, purpleTex); 
}
   


void kx::ClearPhysics ()
{
    std::cout<< "ClearPhysics()" << std::endl;
    // delete all constraints, collision objects, rigid bodies, shapes
    // before creating new game or app quit
    // TODO: class destructors should take care of all delete's !?

    s32 i;

	// delete constraints
	for (i=physics->getNumConstraints()-1; i>=0; i--)
	{
		btTypedConstraint* constraint = physics->getConstraint (i);
		physics->removeConstraint (constraint);
		delete constraint;
	}

	//remove the rigidbodies from the dynamics world and delete them
	for (i=physics->getNumCollisionObjects()-1; i>=0; i--)
	{
		btCollisionObject* obj = physics->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast (obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		physics->removeCollisionObject (obj);
		delete obj;
	}

	//delete collision shapes
    /*
	for (i=0; i<collisionShapes.size(); i++)
	{
		btCollisionShape* shape = collisionShapes[i];
		delete shape;
	}
	collisionShapes.clear();
    */
}


// Converts a Bullet quaternion to an Irrlicht euler angle
void kx::QuaternionToEuler (const btQuaternion &TQuat, btVector3 &TEuler) 
{
	btScalar W = TQuat.getW();
	btScalar X = TQuat.getX();
	btScalar Y = TQuat.getY();
	btScalar Z = TQuat.getZ();
	float WSquared = W * W;
	float XSquared = X * X;
	float YSquared = Y * Y;
	float ZSquared = Z * Z;

	TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
	TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
	TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
	TEuler *= core::RADTODEG;
}


// Convert btVector3 to irr:core::vector3df
vector3df kx::Bt2irrVec3 (const btVector3& v)
{
   return vector3df (v.x(),v.y(),v.z()); 
}


