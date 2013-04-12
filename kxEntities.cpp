
//#include "kx.h"
#include "kxPlayerCube.h"
#include "kxLevel.h"
#include "kxKillCube.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void kx::ProcessEntities()
{
    if( now - activeLevel->startTime < 100 ) return;
   
    for( u32 k=0; k<activeLevel->killCubes.size(); k++ )
    {
        kxKillCube* kc= activeLevel->killCubes[k];
        if( kc->isFractured ) continue;
        if( now - kc->lastImpTime < 7 ) continue; // apply 1 impulse per frame @ 125 fps

        kc->moveDir= playerCube->getAbsolutePosition() - kc->getAbsolutePosition();
        kc->moveDir *= kc->moveSpeed;
        kc->rb->applyCentralImpulse( (const btVector3&) kc->moveDir );
        kc->lastImpTime= now;
    }
}

