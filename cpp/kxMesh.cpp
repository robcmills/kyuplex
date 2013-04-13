
#include "kxMesh.h"

kxMesh::kxMesh( IMesh* mesh ) : kxNode(), edgeMat( SMaterial() )
{
	#ifdef _DEBUG
	setDebugName("kxMesh");
	#endif

    meshNode= k.smgr->addMeshSceneNode( mesh );

    edgeMat.Lighting= false;
    edgeMat.Wireframe= true;
    edgeMat.setTexture( 0, k.darkGrayTex );
}

kxMesh::~kxMesh()
{
	if( meshNode ) meshNode->drop();
}


void kxMesh::OnRegisterSceneNode()
{
	if (IsVisible)
		ISceneNode::OnRegisterSceneNode();
}


void kxMesh::render()
{

}

