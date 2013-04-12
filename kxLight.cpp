#include "kxLight.h"
#include "kxMotionState.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxLight::kxLight(): kxNode(),
	m_radius( 6.4f ), m_bbSize( 1.6f ), m_showRays( false )
{
    //m_edgeMat.Lighting= false;

    lightNode= k.smgr->addLightSceneNode(
        k.smgr->getRootSceneNode(), 
        vector3df(0),
        SColor(255,255,255,255),
        m_radius,
        KNT_LIGHT );
    lightNode->setName( "kxLight" );
    lightNode->enableCastShadow( true );

    // billboard
    bb= k.smgr->addBillboardSceneNode(
        lightNode,
        dimension2df( m_bbSize, m_bbSize ) );
    bb->getMaterial(0).Lighting= false; 
    bb->getMaterial(0).MaterialType= EMT_TRANSPARENT_ADD_COLOR; 
    bb->getMaterial(0).setTexture( 0, k.lightTex );


    // bullet rigid body
    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3( 0,0,0 ));  

    btDefaultMotionState* motionState = new kxMotionState( (ISceneNode*) lightNode );
    motionState->setWorldTransform( t );

    btCollisionShape* shape = new btSphereShape( m_bbSize*.75f/2 ); // kxLight.png circle radius is .75 of bbSize

    btVector3 localInertia;
    btScalar mass( m_bbSize ); 
    shape->calculateLocalInertia( mass, localInertia );

    rb = new btRigidBody( mass, motionState, shape, localInertia );

    //rb->setActivationState (DISABLE_DEACTIVATION);
    rb->setDamping( defLinDamp, defAngDamp );
    rb->setUserPointer( (void*) this ); // weakness?
    rb->setSleepingThresholds( 1,1 );

    k.physics->addRigidBody( rb, ECG_LIGHT, ECG_STATIC | ECG_DYNAMIC | ECG_FRACTURE_CUBE );
    //rb->setActivationState( 0 );

}

kxLight::~kxLight()
{
    bb->drop();
    lightNode->drop();

    if( rb ) k.physics->removeRigidBody( rb );
}

void kxLight::setPosition( const btVector3& pos )
{
    lightNode->setPosition( vector3df( pos.x(),pos.y(),pos.z() ));
    //bb->setPosition( pos );
    if( rb ) 
    {
        rb->getWorldTransform().setOrigin( pos );
        rb->setActivationState( 2 );
    }
}

