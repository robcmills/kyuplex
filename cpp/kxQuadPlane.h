#ifndef __KX_QUAD_PLANE_H_INCLUDED__
#define __KX_QUAD_PLANE_H_INCLUDED__

//#include "kx.h"
#include <irrlicht.h>
#include "kxEnums.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
struct kxQuadFace
{
    u32 ix, iy; // location index 
    array< S3DVertex& > vertices;
    //triangle3df triA, triB;
};
*/


// this plane is constructed with vertices dedicated to each quad face
// to allow vertex coloring with sharp divisions between 
// different colored adjacent faces and animating each quad independently
class kxQuadPlane : public ISceneNode
{
public:
    kxQuadPlane
    ( 
        ISceneNode* parent, ISceneManager* mgr, 
        s32 id= KNT_QUAD_PLANE, 
        dimension2df quadSize= dimension2df( 0.4f,0.4f), 
        dimension2du quadCount= dimension2du( 64,64 ),
        path imgPath= "./textures/defaultQuad64.png"
    );

    virtual void Reset();

    /*
    ~kxQuadPlane() 
    { 
        std::cout<< "\n~kxQuadPlane()";
        if( image ) image->drop();
    }
    */

    virtual void OnRegisterSceneNode();

    virtual void render();

    virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_QUAD_PLANE; }

    virtual u32 getMaterialCount() const { return 1; }
    virtual video::SMaterial& getMaterial( u32 i ) { return Material; }

    virtual void serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options=0 ) const;
    virtual void deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options=0 );


    void ColorizeQuads();

    virtual IImage* getImage() { return Image; }
    //virtual void setImage( IImage* img= 0 ); 

private:
    dimension2df QuadSize;
    dimension2du QuadCount;
    path ImgPath;

    core::aabbox3d<f32> Box;
    array< S3DVertex > Vertices;
    array< u16 > Indices;
    video::SMaterial Material;

    IImage* Image;
};

#endif
