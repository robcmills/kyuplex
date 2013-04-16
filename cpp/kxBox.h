#ifndef __KX_CUBE_NODE_H_INCLUDED__
#define __KX_CUBE_NODE_H_INCLUDED__

#include "kxNode.h"
//#include "kxEnums.h"
//#include <IMeshSceneNode.h>
//#include <SMesh.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxBox : public kxNode
{
public:
    //! constructor
    kxBox( const vector3df& size = vector3df( 1.6f ));
    virtual ~kxBox() {}

    virtual void OnRegisterSceneNode();

    virtual void render();

    virtual const core::aabbox3d<f32>& getBoundingBox() const { return m_box; }

    virtual video::SMaterial& getMaterial( u32 i ) { return m_material; }
    virtual video::SMaterial& getEdgeMaterial() { return m_edgeMat; }
    virtual u32 getMaterialCount() const { return 1; }

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_BOX; }


    virtual void makeMesh(); // creates a new cube
    virtual void setSize( const vector3df& newSize ) { m_size= newSize; makeMesh(); }
    vector3df getSize() const { return m_size; }

    virtual void setPosition( const vector3df& pos ) { ISceneNode::setPosition( pos ); }

    virtual void setColor( const SColor& );
    virtual void setEdgeColor( const SColor& );


    bool m_showNormals;
    vector3df m_size;

    SColor m_color, m_edgeColor;
    SMaterial m_material;
    SMaterial m_edgeMat;

    //core::aabbox3d<f32> m_box;
    array< S3DVertex > m_vertices;
    array< u16 > m_indices;
    array< S3DVertex > m_edgeVertices;
    array< u16 > m_edgeIndices;

    //IMeshSceneNode* meshNode;

};


#endif
