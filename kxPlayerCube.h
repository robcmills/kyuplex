#ifndef __KX_PLAYERCUBE_H_INCLUDED__
#define __KX_PLAYERCUBE_H_INCLUDED__

#include "kxFractureCube.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxPlayerCube : public kxFractureCube
{
public:
    kxPlayerCube();
    virtual ~kxPlayerCube();

    void setPosition( const vector3df& );
    void setPosition( const btVector3& );

    virtual void reset();

    btGeneric6DofSpringConstraint* spring;
    btGhostObject* ghost; // for collision detection

    u16 state;
    vector3df moveDir;
    f32 moveSpeed, speedStep;
    f32 impactDeathThreshold, impactSoundThreshold, velDeathThreshold; 
    u32 lastImpTime;  // last impulse time (for framerate indep movement)

    btScalar linDamp, angDamp;

    s16 integrity; // like hp; 0=die 
};

#endif
