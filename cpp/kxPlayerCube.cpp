
#include "kxPlayerCube.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxPlayerCube::kxPlayerCube(): 
    kxFractureCube(),
    integrity( 1 ),
    state( EPS_CUBE ),
    moveSpeed( 30.f ),
    speedStep( 10.f ),
    moveDir( vector3df( 0,0,0 )),
    impactDeathThreshold( .01f ),
    impactSoundThreshold( .002f ),
    velDeathThreshold( .1f ),
    lastImpTime( k.now ),
    linDamp( 0.5f ),
    angDamp( 0.5f ),
    hasSoundTool( false ),
    activeTool( KT_NONE )
{
    setName( "playerCubeNode" );
    setID( KNT_PLAYER_CUBE );

    //setSize( vector3df( 3.2f,1.6f,.8f ));
    //setColor( SColor( 255,255,150,0 ));
    // sets each face different color
    /*
    u16 i= 0;
    for(i; i<4; i++) { m_vertices[i].Color= SColor(255,0,255,0); }
    for(i=4; i<8; i++) { m_vertices[i].Color= SColor(255,255,0,255); }
    for(i=8; i<12; i++) { m_vertices[i].Color= SColor(255,255,0,0); }
    for(i=12; i<16; i++) { m_vertices[i].Color= SColor(255,0,255,255); }
    for(i=16; i<20; i++) { m_vertices[i].Color= SColor(255,0,0,255); }
    for(i=20; i<24; i++) { m_vertices[i].Color= SColor(255,255,255,0); }
    */

    //m_material.setTexture( 0, k.playerTex );

    //m_edgeMat.setTexture( 0, k.grayTex );

    rb->setActivationState( DISABLE_DEACTIVATION );
    rb->setDamping( linDamp,angDamp );
    //rb->setFlags (btCollisionObject::CF_NO_CONTACT_RESPONSE);
    rb->getBroadphaseHandle()->m_collisionFilterGroup= ECG_PLAYER;
    rb->getBroadphaseHandle()->m_collisionFilterMask= ECG_STATIC | ECG_RIGID_BOX | ECG_FRACTURE_CUBE | ECG_LIGHT;


    ghost = new btGhostObject();
	ghost->setCollisionShape( rb->getCollisionShape() );
	//ghost->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
    k.physics->addCollisionObject( ghost, ECG_GHOST, ECG_COLLECTABLE | ECG_SOUND_TOOL );


    // SPRING CONSTRAINT
    // brings back to equilibrium (axis aligned)
    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3 (0,0,0) );
    //rb->getMotionState()->getWorldTransform(t);
    spring = new btGeneric6DofSpringConstraint( *rb, t, false );

    btScalar linLim = 1;
    btScalar angLim = 10;
    spring->setLinearUpperLimit( btVector3( linLim,linLim,linLim ));
    spring->setLinearLowerLimit( btVector3( -linLim,-linLim,-linLim ));
    spring->setAngularLowerLimit( btVector3( -angLim,-angLim,-angLim ));
    spring->setAngularUpperLimit( btVector3( angLim,angLim,angLim ));

    k.physics->addConstraint( spring ); //true);
    //spring->setDbgDrawSize (1);
    

    btScalar stiff=1;
    btScalar damp=1;

    // linear axii
    /*
    spring->enableSpring (0, true); // x axis
    spring->setStiffness (0, stiff);
    spring->setDamping (3, damp);
    spring->enableSpring (1, true); // y axis
    spring->setStiffness (1, stiff);
    spring->setDamping (4, damp);
    spring->enableSpring (2, true); // z axis
    spring->setStiffness (2, stiff);
    spring->setDamping (5, 1);
    */
    
    // angular axii
    spring->enableSpring( 3, true );
    spring->setStiffness( 3, stiff );
    spring->setDamping( 3, damp );
    spring->enableSpring( 4, true );
    spring->setStiffness( 4, stiff );
    spring->setDamping( 4, damp );
    spring->enableSpring( 5, true );
    spring->setStiffness( 5, stiff );
    spring->setDamping( 5, damp );

    //spring->setEquilibriumPoint (0,0);
    //spring->setEquilibriumPoint (1,0);
    //spring->setEquilibriumPoint (2,0);
    spring->setEquilibriumPoint( 3,0 );
    spring->setEquilibriumPoint( 4,0 );
    spring->setEquilibriumPoint( 5,0 );

    spring->setBreakingImpulseThreshold( 10 );

    /*
    spring->setParam (BT_CONSTRAINT_STOP_CFM,0.8,0);
    spring->setParam (BT_CONSTRAINT_STOP_CFM,0.8,1);
    spring->setParam (BT_CONSTRAINT_STOP_CFM,0.8,2);
    spring->setParam (BT_CONSTRAINT_STOP_CFM,0.8,3);
    spring->setParam (BT_CONSTRAINT_STOP_CFM,0.8,4);
    spring->setParam (BT_CONSTRAINT_STOP_CFM,0.8,5);

    spring->setParam (BT_CONSTRAINT_STOP_ERP,0.1,0);
    spring->setParam (BT_CONSTRAINT_STOP_ERP,0.1,1);
    spring->setParam (BT_CONSTRAINT_STOP_ERP,0.1,2);
    spring->setParam (BT_CONSTRAINT_STOP_ERP,0.1,3);
    spring->setParam (BT_CONSTRAINT_STOP_ERP,0.1,4);
    spring->setParam (BT_CONSTRAINT_STOP_ERP,0.1,5);
    */

}

kxPlayerCube::~kxPlayerCube() 
{
    std::cout<< "~kxPlayerCube()" << std::endl;
    if( spring ) k.physics->removeConstraint( spring );
    if( ghost ) k.physics->removeCollisionObject( ghost );
}

void kxPlayerCube::setPosition( const vector3df& pos )
{
    kxFractureCube::setPosition( pos );
}

void kxPlayerCube::setPosition( const btVector3& pos )
{
    ghost->getWorldTransform().setOrigin( pos ); 
    spring->getFrameOffsetA().setOrigin( pos );

    kxFractureCube::setPosition( pos ); 
}

void kxPlayerCube::reset()
{
    // reset key presses 
    for( u32 i=0; i<k.keyMapArray.size(); ++i )
        k.keyMapArray[i].pressed= false;

    rb->setDamping( linDamp,angDamp );
    spring->setEnabled( true );

    moveDir = vector3df( 0,0,0 );
    lastImpTime= k.now + 100;
    integrity= 1;
    state= EPS_CUBE;
    moveSpeed= 30.f; 

    removeAnimators();
    kxFractureCube::reset();
}

