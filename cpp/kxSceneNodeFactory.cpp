// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "kxSceneNodeFactory.h"
#include "kxBox.h"
#include "kxFractureCube.h"
#include "kxPlayerCube.h"
#include "kxCollectable.h"
#include "kxQuadPlane.h"

using namespace irr;
using namespace core;
using namespace scene;


//namespace irr
//{
//namespace scene
//{


kxSceneNodeFactory::kxSceneNodeFactory( ISceneManager* mgr )
: Manager( mgr )
{

	#ifdef _DEBUG
	setDebugName("kxSceneNodeFactory");
	#endif

	// don't grab the scene manager here to prevent cyclic references

	kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_ROOT_NODE, "rootNode" ));
	kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_BOX, "kxBox" ));
	kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_FRACTURE_CUBE, "kxFractureCube" ));
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_PLAYER_CUBE, "kxPlayerCube" ));
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_COLLECTABLE, "kxCollectable" ));
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_QUAD_PLANE, "kxQuadPlane" ));
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( (ESCENE_NODE_TYPE) KNT_SPLINE_NODE, "kxSpline" ));
    /*
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( KNT_TESTCUBE, 
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( KNT_SHELLCUBE, 
	kxSceneNodeTypes.push_back( SSceneNodeTypePair( KNT_LEVEL,
    kxSceneNodeTypes.push_back( SSceneNodeTypePair( KNT_ENTITY_LIGHT
    */

}


//! adds a scene node to the scene graph based on its type id
ISceneNode* kxSceneNodeFactory::addSceneNode( ESCENE_NODE_TYPE type, ISceneNode* parent )
{
	switch( type )
	{
	case KNT_BOX:
		return static_cast< ISceneNode* >( new kxBox() );

	case KNT_FRACTURE_CUBE:
		return static_cast< ISceneNode* >( new kxFractureCube() );

	case KNT_PLAYER_CUBE:
		return static_cast< ISceneNode* >( new kxPlayerCube() );

	case KNT_COLLECTABLE:
		return static_cast< ISceneNode* >( new kxCollectable() );
        
	case KNT_QUAD_PLANE:
		return static_cast< ISceneNode* >( new kxQuadPlane( parent, Manager ));

	default:
		break;
	}

	return 0;
}


//! adds a scene node to the scene graph based on its type name
ISceneNode* kxSceneNodeFactory::addSceneNode( const c8* typeName, ISceneNode* parent )
{
	return addSceneNode( getTypeFromName( typeName ), parent );
}


//! returns amount of scene node types this factory is able to create
u32 kxSceneNodeFactory::getCreatableSceneNodeTypeCount() const
{
	return kxSceneNodeTypes.size();
}


//! returns type of a createable scene node type
ESCENE_NODE_TYPE kxSceneNodeFactory::getCreateableSceneNodeType(u32 idx) const
{
	if (idx<kxSceneNodeTypes.size())
		return kxSceneNodeTypes[idx].Type;
	else
		return ESNT_UNKNOWN;
}


//! returns type name of a createable scene node type
const c8* kxSceneNodeFactory::getCreateableSceneNodeTypeName(u32 idx) const
{
	if( idx<kxSceneNodeTypes.size() )
		return kxSceneNodeTypes[idx].TypeName.c_str();
	else
		return 0;
}


//! returns type name of a createable scene node type
const c8* kxSceneNodeFactory::getCreateableSceneNodeTypeName( ESCENE_NODE_TYPE type ) const
{
	for( u32 i=0; i<kxSceneNodeTypes.size(); ++i )
		if( kxSceneNodeTypes[i].Type == type )
			return kxSceneNodeTypes[i].TypeName.c_str();

	return 0;
}


ESCENE_NODE_TYPE kxSceneNodeFactory::getTypeFromName( const c8* name ) const
{
	for( u32 i=0; i<kxSceneNodeTypes.size(); ++i )
		if( kxSceneNodeTypes[i].TypeName == name )
			return kxSceneNodeTypes[i].Type;

	return ESNT_UNKNOWN;
}


//} // end namespace scene
//} // end namespace irr

