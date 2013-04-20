#ifndef __KX_LEVEL_H_INCLUDED__
#define __KX_LEVEL_H_INCLUDED__

#include "kx.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//class kx;
class kxCollectable;
class kxQuadPlane;
class kxSpline;
class kxKillCube;
class kxFractureCube;
class kxBox;
class kxRigidBox;
class kxMesh;
class kxLight;


class kxLevel
{
public:
    kxLevel
    ( 
        u32 num= 1,
        u32 startTime= 0,
        u32 progress= 0,
        u32 progressTotal= 10,
        vector3df gravity= vector3df( 0,0,0 )
    );
    virtual ~kxLevel();

    void reset();

    u32 num,   // level number
        startTime, 
        progress,  
        progressTotal;
    vector3df gravity;

    // LEVEL GEOMETRY
    array< kxQuadPlane* > quadPlaneArray;
    array< kxSpline* > splineArray;
    //array< kxMesh* > meshArray;
    array< IAnimatedMeshSceneNode* > meshArray;

    // PHYSICS OBJECTS
    //array< kxCollisionCube* > ccArray;
    btAlignedObjectArray< btCollisionObject* > coArray;
    //btAlignedObjectArray< btRigidBody* > rbArray;

    // ENTITIES
    array< kxLight* > lights;
    array< kxCollectable* > collectables;
    array< kxKillCube* > killCubes;
    array< kxFractureCube* > fractureCubes;
    array< kxBox* > boxes;
    array< kxRigidBox* > rboxes;
    u32 numActiveCollectables;
    
    //virtual int LevelCallback ();

protected:
    kx& k; // context
};

#endif
