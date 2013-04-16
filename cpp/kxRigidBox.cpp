
#include "kxRigidBox.h"
#include "kxMotionState.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxRigidBox::kxRigidBox( const vector3df& size ):
    kxBox( size ), 
    rb( 0 ),
    defLinDamp( .5f ), defAngDamp( .5f )
{
    setName( "kxRigidBox" );
    setID( KNT_RIGID_BOX );

    //m_mesh->getMeshBuffer(0)->getMaterial().setTexture( 0, k.grayTex );
    //m_material.Lighting= true; 
    //m_material.MaterialType= EMT_SOLID; 

    //m_edgeMat.setTexture( 0, k.darkGrayTex );
    //m_edgeMat.MaterialType= EMT_TRANSPARENT_ADD_COLOR;

    // bullet rigid body
    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3( 0,0,0 ));  

    btDefaultMotionState* motionState = new kxMotionState( (ISceneNode*) this );
    motionState->setWorldTransform( t );

    // collision shape will be a unit cube 
    // and we will use scaling to adjust for dynamic size changes
    btVector3 halfExtents( .5f, .5f, .5f );
    btCollisionShape* shape = new btBoxShape( halfExtents );
    shape->setLocalScaling( btVector3( m_size.X, m_size.Y, m_size.Z ));

    btVector3 localInertia;
    btScalar mass( (m_size.X+m_size.Y+m_size.Z)/3 ); 
    shape->calculateLocalInertia( mass, localInertia );

    rb = new btRigidBody( mass, motionState, shape, localInertia );

    //rb->setActivationState (DISABLE_DEACTIVATION);
    rb->setDamping( defLinDamp, defAngDamp );
    rb->setUserPointer( (void*) this );
    rb->setSleepingThresholds( 1,1 );

    k.physics->addRigidBody( rb, ECG_RIGID_BOX, ECG_STATIC | ECG_DYNAMIC | ECG_RIGID_BOX | ECG_FRACTURE_CUBE | ECG_LIGHT );
    //rb->setActivationState( 0 );
}

kxRigidBox::~kxRigidBox() 
{
    std::cout<< "~kxRigidBox()" << std::endl;
    if( rb ) k.physics->removeRigidBody( rb );
}


void kxRigidBox::setPosition( const vector3df& pos )
{
    kxBox::setPosition( pos );
}

// setPos needs overloading to avoid cyclic setting of transforms ??
void kxRigidBox::setPosition( const btVector3& pos )
{
    if( rb ) 
    {
        rb->getWorldTransform().setOrigin( pos );
        rb->setActivationState( 2 );
    }
    kxBox::setPosition( vector3df( pos.x(),pos.y(),pos.z() ));
}

void kxRigidBox::setSize( const vector3df& newSize )
{
    rb->getCollisionShape()->setLocalScaling( btVector3( newSize.X, newSize.Y, newSize.Z ));
    btVector3 localInertia;
    btScalar newMass( (newSize.X+newSize.Y+newSize.Z)/3 );
    rb->getCollisionShape()->calculateLocalInertia( newMass, localInertia );
    rb->setMassProps( newMass, localInertia );
    rb->updateInertiaTensor();

    kxBox::setSize( newSize );
}

