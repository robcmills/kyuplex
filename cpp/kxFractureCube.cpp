
#include "kxFractureCube.h"
#include "kxMotionState.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxFractureCube::kxFractureCube():
    kxBox(), 
    isFractured( false ),
    defLinDamp( .5f ), defAngDamp( .5f )
{
    setName( "kxFractureCube" );
    setID( KNT_FRACTURE_CUBE );

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

    k.physics->addRigidBody( rb, ECG_FRACTURE_CUBE, ECG_STATIC | ECG_DYNAMIC | ECG_RIGID_BOX | ECG_FRACTURE_CUBE | ECG_LIGHT );
    //rb->setActivationState( 0 );
}

kxFractureCube::~kxFractureCube() 
{
    std::cout<< "~kxFractureCube()" << std::endl;
    for( u32 f=0; f<fractureArray.size(); f++)
        fractureArray[f]->remove();
    fractureArray.clear();

    if( rb ) k.physics->removeRigidBody( rb );
}

/*
void kxFractureCube::OnRegisterSceneNode()
{
	if (IsVisible)
        k.SyncTransform( this, rb->getWorldTransform() );

	kxBox::OnRegisterSceneNode();
}
*/


void kxFractureCube::setPosition( const vector3df& pos )
{
    kxBox::setPosition( pos );
}

// setPos needs overloading to avoid cyclic setting of transforms
void kxFractureCube::setPosition( const btVector3& pos )
{
    if( rb ) 
    {
        rb->getWorldTransform().setOrigin( pos );
        rb->setActivationState( 2 );
    }
    kxBox::setPosition( vector3df( pos.x(),pos.y(),pos.z() ));
}

void kxFractureCube::setSize( const vector3df& newSize )
{
    if( isFractured ) return;
    //rb->getCollisionShape()->setLocalScaling( btVector3( newSize.X/m_size.X, newSize.Y/m_size.Y, newSize.Z/m_size.Z ));
    rb->getCollisionShape()->setLocalScaling( btVector3( newSize.X, newSize.Y, newSize.Z ));
    btVector3 localInertia;
    btScalar newMass( (newSize.X+newSize.Y+newSize.Z)/3 );
    rb->getCollisionShape()->calculateLocalInertia( newMass, localInertia );
    rb->setMassProps( newMass, localInertia );
    rb->updateInertiaTensor();

    kxBox::setSize( newSize );
}


void kxFractureCube::Fracture( vector3df n )
{
    if( isFractured ) return;

    isFractured = true;

    // Fracture into n^3 cubes 
    if (n.X > 6.f) n.X= 6.f;  // not too much
    if (n.Y > 6.f) n.Y= 6.f;  
    if (n.Z > 6.f) n.Z= 6.f; 

    // Position cubes and set/transfer props
    const btScalar adjustx= m_size.X/2.f - m_size.X/n.X/2.f;
    const btScalar adjusty= m_size.Y/2.f - m_size.Y/n.Y/2.f;
    const btScalar adjustz= m_size.Z/2.f - m_size.Z/n.Z/2.f;
    const btScalar angDamp= 0.2f;
    const btScalar linDamp= 0.2f;
    const btScalar rest= 1;
    const btVector3 linV= rb->getLinearVelocity();
    const btVector3 angV= rb->getAngularVelocity();
    const btVector3 o= rb->getWorldTransform().getOrigin();   

    const vector3df newSize( m_size.X/n.X, m_size.Y/n.Y, m_size.Z/n.Z );

    // sub-cubes will share one collision shape?
    //const vector3df newSize( cube->m_size.X/(f32)n, cube->m_size.Y/(f32)n, cube->m_size.Z/(f32)n );
    //cube->rb->getCollisionShape()->setLocalScaling( btVector3( newSize.X/cube->m_size.X, newSize.Y/cube->m_size.Y, newSize.Z/cube->m_size.Z ));

    for (u32 x=0; x<n.X; x++) {
        for (u32 y=0; y<n.Y; y++) {
            for (u32 z=0; z<n.Z; z++) {
                kxFractureCube* cube = new kxFractureCube();

                //cube->m_material.setTexture( 0, k.grayTex );
                //cube->m_material.Lighting= true; 
                //cube->m_material.MaterialType= EMT_TRANSPARENT_ADD_COLOR; 
                cube->setSize( newSize );

                const btVector3 newPos(
                    o.x() + x*cube->m_size.X - adjustx,
                    o.y() + y*cube->m_size.Y - adjusty,
                    o.z() + z*cube->m_size.Z - adjustz
                );
                cube->setPosition( newPos );
                cube->rb->setDamping( linDamp, angDamp );
                cube->rb->setRestitution( rest );
                cube->rb->setLinearVelocity( linV );
                cube->rb->setAngularVelocity( angV );

                fractureArray.push_back( cube );
            }
        }
    }

    // Scaling down the cs creates 'explode' effect, 
    // pushing fracture sub-cubes away;
    // This avoids boring fractures where cubes stay together
    rb->getCollisionShape()->setLocalScaling( btVector3( .01f,.01f,.01f ));

    //rb->getBroadphaseHandle()->m_collisionFilterGroup= ECG_NULL;
    rb->setDamping( .8f, .8f );
    setVisible( false );
    //getMaterial(0).setTexture( 0, k.grayTex );
}

void kxFractureCube::reset()
{
    isFractured= false;

    for( u32 f=0; f<fractureArray.size(); f++)
    {
        k.physics->removeRigidBody( fractureArray[f]->rb );
        fractureArray[f]->remove();
    }
    fractureArray.clear();

    btTransform t;
    t.setIdentity();
    t.setOrigin( btVector3( 0,0,0 ));
    rb->setWorldTransform( t );
    //rb->getBroadphaseHandle()->m_collisionFilterGroup= ECG_STATIC | ECG_DYNAMIC | ECG_FRACTURE_CUBE;
    rb->getCollisionShape()->setLocalScaling( btVector3( m_size.X, m_size.Y, m_size.Z ));
    //rb->setDamping( 0.5f,0.5f );
    rb->setLinearVelocity( btVector3(0,0,0));
    rb->setAngularVelocity( btVector3(0,0,0));
    rb->clearForces();
    rb->updateInertiaTensor();

    setPosition( btVector3(0,0,0) );
    setPosition( vector3df(0,0,0) );
    setVisible( true );
}


