
#include "kxQuadPlane.h"
#include <iostream>

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxQuadPlane::kxQuadPlane
(
    ISceneNode* parent, ISceneManager* mgr, 
    s32 id, 
    dimension2df qs,
    dimension2du qc,
    path ip
): 
    ISceneNode( parent, mgr, id ),
    QuadSize( qs ), QuadCount( qc ),
    ImgPath( ip ), Image( 0 )
{
    Reset();

    //Material.setTexture( 0, SceneManager->getVideoDriver()->getTexture( "./textures/squares4gradient.png" ));
    //Material.Lighting= false;
    //Material.AntiAliasing= EAAM_QUALITY; //|EAAM_LINE_SMOOTH );
    ISceneNode::setName( "kxQuadPlane" );

}

void kxQuadPlane::Reset()
{
    Vertices.clear();
    Indices.clear();

    // check for bad values?

    // vertices 
    // clockwise winding

    //   0--------1/4--------5
    //   |         |         |
    //   |         |         |
    //   |         |         |
    //   3________2/7________6   
    //   8        9/12      13
    //   |         |         |
    //   |         |         |
    //   |         |         |
    //   11------10/15------14

	video::S3DVertex v;
	v.Color.set(255,155,155,155);
    v.Normal.set( vector3df( 0,1,0 ));               
    
    for (f32 z=0.f; z<QuadCount.Height; z++)
        for (f32 x=0.f; x<QuadCount.Width; x++)
        {
            // 4 vertices per Quad
            // 0
            v.Pos.set( x*QuadSize.Width, 0, -z*QuadSize.Height );                
            v.TCoords.set( vector2df( x/QuadCount.Width, z/QuadCount.Height ));  
            Vertices.push_back( v );
            // 1
            v.Pos.set( (x+1)*QuadSize.Width, 0, -z*QuadSize.Height );                
            v.TCoords.set( vector2df( (x+1)/QuadCount.Width, z/QuadCount.Height ));  
            Vertices.push_back( v );
            // 2
            v.Pos.set( (x+1)*QuadSize.Width, 0, -(z+1)*QuadSize.Height );                
            v.TCoords.set( vector2df( (x+1)/QuadCount.Width, (z+1)/QuadCount.Height ));  
            Vertices.push_back( v );
            // 3
            v.Pos.set( x*QuadSize.Width, 0, -(z+1)*QuadSize.Height );                
            v.TCoords.set( vector2df( x/QuadCount.Width, (z+1)/QuadCount.Height ));  
            Vertices.push_back( v );

        }

	// indices
    for (u16 i=0; i<QuadCount.Width*QuadCount.Height*4; i++)
        Indices.push_back( i );

    Box= aabbox3df( Vertices[0].Pos, Vertices[ Vertices.size()-1 ].Pos );

    if( Image ) Image->drop();
    Image= SceneManager->getVideoDriver()->createImageFromFile( ImgPath );
    if( Image ) ColorizeQuads();
}

void kxQuadPlane::OnRegisterSceneNode()
{
    if( IsVisible )
        SceneManager->registerNodeForRendering( this );

    ISceneNode::OnRegisterSceneNode();
}

void kxQuadPlane::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setMaterial( Material );
    driver->setTransform( ETS_WORLD, AbsoluteTransformation );
    driver->drawVertexPrimitiveList(
        &Vertices[0], Vertices.size(), 
        &Indices[0], 
        QuadCount.Width*QuadCount.Height, // primitive count
        video::EVT_STANDARD, scene::EPT_QUADS, 
        video::EIT_16BIT
    );
}

//! Writes attributes of the scene node.
void kxQuadPlane::serializeAttributes( io::IAttributes* out, io::SAttributeReadWriteOptions* options ) const
{
	ISceneNode::serializeAttributes( out, options );

    out->addFloat( "QuadSizeWidth", QuadSize.Width );  
    out->addFloat( "QuadSizeHeight", QuadSize.Height );  
    out->addInt( "QuadCountWidth", QuadCount.Width );  
    out->addInt( "QuadCountHeight", QuadCount.Height );  
    out->addString( "ImgPath", ImgPath.c_str() );  
}


//! Reads attributes of the scene node.
void kxQuadPlane::deserializeAttributes( io::IAttributes* in, io::SAttributeReadWriteOptions* options )
{
    f32 qsw= in->getAttributeAsFloat( "QuadSizeWidth" );
    f32 qsh= in->getAttributeAsFloat( "QuadSizeHeight" );
    u32 qcw= in->getAttributeAsInt( "QuadCountWidth" );
    u32 qch= in->getAttributeAsInt( "QuadCountHeight" );
    stringc imgPath= in->getAttributeAsString( "ImgPath" );
    
    QuadSize= dimension2df( qsw, qsh );
    QuadCount= dimension2du( qcw, qch );
    ImgPath= imgPath.c_str();

    Reset();

	ISceneNode::deserializeAttributes( in, options );
}

/*
void kxQuadPlane::setImage( path newPath )
{
    if( ImgPath != newPath )
    {
        ImgPath= newPath;
        IImage* newImg= SceneManager->getVideoDriver()->createImageFromFile( imgPath );
        if( newImage ) 
        {
            delete Image;
            Image= newImage;
            ColorizeQuads();
        }
    }
}
*/


void kxQuadPlane::ColorizeQuads()
{
    if( Image )
        for (u32 z=0; z<QuadCount.Height; z++)
            for (u32 x=0; x<QuadCount.Width; x++)
                for (u32 q=0; q<4; q++)
                    Vertices[ x*4+z*4*QuadCount.Width+q ].Color= Image->getPixel( x,z );
}
