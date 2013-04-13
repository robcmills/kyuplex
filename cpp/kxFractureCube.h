#ifndef __KX_FRACTURE_CUBE_H_INCLUDED__
#define __KX_FRACTURE_CUBE_H_INCLUDED__

#include "kxBox.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxFractureCube : public kxBox
{
public:
    kxFractureCube();
    virtual ~kxFractureCube();

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_FRACTURE_CUBE; }

    //virtual void OnRegisterSceneNode();

    virtual void setPosition( const vector3df& pos );
    virtual void setPosition( const btVector3& pos );

    //void Fracture( u16 n= 2 );    
    void Fracture( vector3df n= vector3df(2.f) );    

    virtual void reset(); 

    virtual void setSize( const vector3df& );

    bool isFractured;

    btRigidBody* rb;
    // default linear angular damping
    btScalar defLinDamp, defAngDamp;

    array< kxFractureCube* > fractureArray;
};

#endif
