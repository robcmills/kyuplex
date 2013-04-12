
#ifndef __KX_MESH_H_INCLUDED__
#define __KX_MESH_H_INCLUDED__

#include "kxNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxMesh : public kxNode
{
public:

    kxMesh( IMesh* mesh );
    virtual ~kxMesh();

    virtual void OnRegisterSceneNode();

    virtual void render();

    virtual video::SMaterial& getEdgeMaterial() { return edgeMat; }

    virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) KNT_MESH; }


    IMeshSceneNode* meshNode;
    SMaterial edgeMat;
};

#endif

