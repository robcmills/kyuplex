
#include "kxCollectable.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxCollectable::kxCollectable(): 
    kxBox(),
    isCollected( false )
    //cSound( k.sineF3 ) // default collected sound
{
    //setName( "kxCollectable" );
    //setID( KNT_COLLECTABLE );
    setSize( vector3df( .8f ));
    setColor( SColor(255,0,255,0) );
    setEdgeColor( SColor(255,255,255,255) );
    //m_material.setTexture( 0, k.greenTex );
    //m_material.Lighting= false; 
    m_material.MaterialType= EMT_TRANSPARENT_ADD_COLOR; 

    // collision object
    btVector3 halfExtents( m_size.X/2.f, m_size.Y/2.f, m_size.Z/2.f );
    btCollisionShape* shape = new btBoxShape( halfExtents );

    co = new btCollisionObject();
    co->setCollisionShape( shape );
    co->setUserPointer( (void*) this );

             //addCollisionObject( co, COLLISION_GROUP, COLLISION_MASK )
    k.physics->addCollisionObject( co, ECG_COLLECTABLE, ECG_GHOST );
}


void kxCollectable::setPosition( const vector3df& pos )
{
    co->getWorldTransform().setOrigin( btVector3( pos.X,pos.Y,pos.Z ));
    ISceneNode::setPosition( pos );
}

void kxCollectable::setRotation( const vector3df& rot )
{
    //TODO: the conversion to quat is not accurate by 4 degrees!
    //std::cout<< "vector3df rotation:"<< rot.Y << rot.X << rot.Z <<std::endl;
    const btQuaternion q( rot.Y, rot.X, rot.Z );
    //std::cout<< "btQuat rotation:"<< q.y() << q.x() << q.z() <<std::endl;
    co->getWorldTransform().setRotation( q );

    ISceneNode::setRotation( rot );
}
