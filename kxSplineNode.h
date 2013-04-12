#ifndef __KX_SPLINE_NODE_H_INCLUDED__
#define __KX_SPLINE_NODE_H_INCLUDED__

#include "kxNode.h"
//#include <irrlicht.h>
//#include "kxEnums.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxSpline : public kxNode
{
public:
    kxSpline
    ( 
        //ISceneNode* parent, ISceneManager* mgr, s32 id, 
        array< S3DVertex >& vertices,
        bool isOpen,
        SMaterial& mat
    );

    virtual void OnRegisterSceneNode();

    virtual void render();

    virtual const core::aabbox3d<f32>& getBoundingBox() const { return m_box; }

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_SPLINE_NODE; }

    virtual u32 getMaterialCount() const { return 1; }
    virtual video::SMaterial& getMaterial( u32 i ) { return m_material; }

    //virtual void serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options=0 ) const;
    //virtual void deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options=0 );

private:
    //core::aabbox3d<f32> m_box;
    array< S3DVertex > m_vertices;
    array< u16 > m_indices;
    video::SMaterial m_material;
};

#endif
