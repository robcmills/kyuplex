
#include "kxNode.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxNode::kxNode():
    k( kx::getInstance() ),
	ISceneNode( kx::getInstance().smgr->getRootSceneNode(), kx::getInstance().smgr ),
    m_box( aabbox3df() )
{
	#ifdef _DEBUG
	setDebugName( "kxNode" );
	#endif
    setName( "kxNode" );
}

