#ifndef __KX_RIGID_BOX_H_INCLUDED__
#define __KX_RIGID_BOX_H_INCLUDED__

#include "kxBox.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxRigidBox : public kxBox
{
public:
    kxRigidBox( const vector3df& size = vector3df( 3.2f ));
    virtual ~kxRigidBox();

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_RIGID_BOX; }

    //virtual void OnRegisterSceneNode();

    virtual void setPosition( const vector3df& pos );
    virtual void setPosition( const btVector3& pos );

    virtual void setSize( const vector3df& );

    btRigidBody* rb;
    // default linear angular damping
    btScalar defLinDamp, defAngDamp;
};

#endif
