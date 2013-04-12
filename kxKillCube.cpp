
#include "kxKillCube.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxKillCube::kxKillCube(): 
    kxFractureCube(),
    integrity( 1 ),
    state( 0 ),
    moveSpeed( .001f ),
    moveDir( vector3df( 0,0,0 )),
    impactDeathThreshold( .01f ),
    impactSoundThreshold( .002f ),
    velDeathThreshold( .1f ),
    lastImpTime( k.now ),
    linDamp( 0.5f ),
    angDamp( 0.5f )
{
    rb->setActivationState( DISABLE_DEACTIVATION );
    rb->setDamping( linDamp,angDamp );
    //rb->getBroadphaseHandle()->m_collisionFilterGroup= ECG_STATIC | ECG_DYNAMIC | ECG_FRACTURE_CUBE;
    
    setName( "kxKillCubeNode" );
    setID( KNT_KILL_CUBE );
    m_material.setTexture( 0, k.redTex ); 

    //rb->setFlags (btCollisionObject::CF_NO_CONTACT_RESPONSE);

}

kxKillCube::~kxKillCube() 
{
    std::cout<< "~kxKillCube()" << std::endl;
}

void kxKillCube::setPosition( const vector3df& pos )
{
    kxFractureCube::setPosition( pos );
}

void kxKillCube::setPosition( const btVector3& pos )
{
    kxFractureCube::setPosition( pos ); 
}

void kxKillCube::reset()
{
    rb->setDamping( linDamp,angDamp );

    moveDir = vector3df( 0,0,0 );
    lastImpTime= k.now + 100;
    integrity= 1;
    state= 0;
    moveSpeed= 30.f; 

    kxFractureCube::reset();
}

