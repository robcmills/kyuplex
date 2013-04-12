#ifndef __KX_COLLECTABLE_H_INCLUDED__
#define __KX_COLLECTABLE_H_INCLUDED__

//#include "kx.h"
#include "kxBox.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxCollectable : public kxBox
{
public:
    kxCollectable();
    virtual ~kxCollectable() {}

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_COLLECTABLE; }

    virtual void setPosition( const vector3df& );
    virtual void setRotation( const vector3df& );

    //s32 Progress;
    //ISoundSource* cSound; // collected sound
    bool isCollected;

    btCollisionObject* co;
};

#endif
