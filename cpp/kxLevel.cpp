#include "kxEnums.h"
#include "kxLevel.h"
#include "kxQuadPlane.h"
#include "kxSpline.h"
#include "kxCollectable.h"
#include "kxKillCube.h"
#include "kxFractureCube.h"
#include "kxBox.h"
#include "kxRigidBox.h"
#include "kxMesh.h"
#include "kxLight.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxLevel::kxLevel
( 
    u32 ln,
    u32 start,
    u32 pro,
    u32 proTot,
    vector3df grav
) : 
    k( kx::getInstance() ),
    num( ln ),
    startTime( start ), 
    progress( pro ), 
    progressTotal( proTot ), 
    gravity( grav ),
    numActiveCollectables( 0 )
{ }

kxLevel::~kxLevel() 
{
    std::cout<< "~kxLevel()" << std::endl;

    // mesh nodes
    for( u32 m=0; m<meshArray.size(); m++ )
        meshArray[m]->drop();
    meshArray.clear();

     // spline nodes
    for( u32 s=0; s<splineArray.size(); s++ )
        splineArray[s]->drop();
    splineArray.clear();
   
    // quad planes
    for( u32 q=0; q<quadPlaneArray.size(); q++)
        quadPlaneArray[q]->drop();
    quadPlaneArray.clear();

    // collision objects
    for( s32 co=0; co<coArray.size(); co++)
    {
        k.physics->removeCollisionObject( coArray[co] );
        delete coArray[co];
    }
    coArray.clear();

    // lights
    for( u32 l=0; l<lights.size(); l++ )
        delete lights[l];
    lights.clear();

    // collectables
    for( u32 c=0; c<collectables.size(); c++ )
        collectables[c]->drop();
    collectables.clear();

    // killCubes
    for( u32 k=0; k<killCubes.size(); k++ )
        killCubes[k]->drop();
    killCubes.clear();

    // fractureCubes
    for( u32 fc=0; fc<fractureCubes.size(); fc++ )
        fractureCubes[fc]->drop();
    fractureCubes.clear();

    // boxes
    for( u32 bx=0; bx<boxes.size(); bx++ )
        boxes[bx]->drop();
    boxes.clear();
    
    // rigid boxes
    for( u32 rbx=0; rbx<rboxes.size(); rbx++ )
        rboxes[rbx]->drop();
    boxes.clear();
}

void kxLevel::reset()
{
    // mesh nodes
    for( u32 m=0; m<meshArray.size(); m++ )
        meshArray[m]->remove();
    meshArray.clear();

     // spline nodes
    for( u32 s=0; s<splineArray.size(); s++ )
        splineArray[s]->remove();
    splineArray.clear();

    // collectables
    for( u32 c=0; c<collectables.size(); c++ )
        collectables[c]->remove();
    collectables.clear();

    // killCubes
    for( u32 kc=0; kc<killCubes.size(); kc++ )
    {
        k.physics->removeRigidBody( killCubes[kc]->rb );
        killCubes[kc]->remove();
    }
    killCubes.clear();
    
    // fractureCubes
    for( u32 fc=0; fc<fractureCubes.size(); fc++ )
    {
        k.physics->removeRigidBody( fractureCubes[fc]->rb );
        fractureCubes[fc]->remove();
    }
    fractureCubes.clear();

    // boxes
    for( u32 bx=0; bx<boxes.size(); bx++ )
        boxes[bx]->remove();
    boxes.clear();

    // rigid boxes
    for( u32 rbx=0; rbx<rboxes.size(); rbx++ )
        rboxes[rbx]->remove();
    rboxes.clear();

    // lights
    for( u32 l=0; l<lights.size(); l++ )
    {
        k.physics->removeRigidBody( lights[l]->rb );
        lights[l]->lightNode->remove();
        //delete lights[l]; // throws ref counting error?
    }
    lights.clear();

}

