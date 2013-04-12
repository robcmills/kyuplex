#ifndef __KX_NODE_H_INCLUDED__
#define __KX_NODE_H_INCLUDED__

#include "kx.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class kxNode : public ISceneNode
{
public:
    kxNode();
    virtual ~kxNode() {}

    virtual void OnRegisterSceneNode() {}
    virtual void render() {}
    virtual const core::aabbox3d<f32>& getBoundingBox() const { return m_box; }

    aabbox3df m_box;
    kx& k; // context
};

#endif
