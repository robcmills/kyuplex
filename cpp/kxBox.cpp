#include "kxBox.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxBox::kxBox( const vector3df& size ): kxNode(), 
	m_size( size ), m_showNormals( false ),
    m_material( SMaterial() ), m_edgeMat( SMaterial() ),
    m_color( SColor(255,150,150,150) ), m_edgeColor( SColor(255,50,50,50) )
{
	#ifdef _DEBUG
	setDebugName( "kxBox" );
	#endif
    setName( "kxBox" );

    //m_material.setTexture( 0, k.whiteTex );
    //m_material.SpecularColor= SColor( 255,0,0,255 );
    //m_material.DiffuseColor= SColor( 255,0,0,255 );
	m_material.Lighting= true;
    m_material.GouraudShading= false; 
    m_edgeMat.Lighting= false;

    makeMesh();
}


void kxBox::makeMesh()
{
	/*
     * 24 vertices; each face gets it's own 4 with normals
     * clockwise winding
                     
       6.11.17-----------0.8.16  
            /|          /|        +y
           / |         / |        ^   +z
          /  |        /  |        |  /
   7.10.20-----------3.9.21       | /
         |   |       |   |        |/
       5.13.18-------|---1.12.19  *----->+x
         |  /        |  / 
         | /         | / 
         |/          |/ 
   4.14.23-----------2.15.22

	*/
    // indices
    //const u16 triangles[36] = { 0,1,3, 1,2,3, 4,5,6, 4,6,7, 8,9,11, 9,10,11, 12,13,15, 13,14,15, 16,17,18, 16,18,19, 20,21,22, 20,22,23 };

    // vertex array
    const f32 x= m_size.X/2;
    const f32 y= m_size.Y/2;
    const f32 z= m_size.Z/2;
    const f32 va[192] = 
    //   Pos          Normals      TCoords   Vertex  Face
    {
         x,  y,  z,   1,  0,  0,   1, 0,     // 0    +x
         x, -y,  z,   1,  0,  0,   1, 1,     // 1
         x, -y, -z,   1,  0,  0,   0, 1,     // 2
         x,  y, -z,   1,  0,  0,   0, 0,     // 3

        -x, -y, -z,  -1,  0,  0,   1, 1,     // 4    -x
        -x, -y,  z,  -1,  0,  0,   0, 1,     // 5
        -x,  y,  z,  -1,  0,  0,   0, 0,     // 6
        -x,  y, -z,  -1,  0,  0,   1, 0,     // 7 

         x,  y,  z,   0,  1,  0,   1, 0,     // 8    +y
         x,  y, -z,   0,  1,  0,   1, 1,     // 9
        -x,  y, -z,   0,  1,  0,   0, 1,     // 10
        -x,  y,  z,   0,  1,  0,   0, 0,     // 11 

         x, -y,  z,   0, -1,  0,   0, 0,     // 12   -y
        -x, -y,  z,   0, -1,  0,   1, 0,     // 13
        -x, -y, -z,   0, -1,  0,   1, 1,     // 14
         x, -y, -z,   0, -1,  0,   0, 1,     // 15

         x,  y,  z,   0,  0,  1,   0, 0,     // 16   +z
        -x,  y,  z,   0,  0,  1,   1, 0,     // 17
        -x, -y,  z,   0,  0,  1,   1, 1,     // 18
         x, -y,  z,   0,  0,  1,   0, 1,     // 19

        -x,  y, -z,   0,  0, -1,   1, 1,     // 20   -z
         x,  y, -z,   0,  0, -1,   0, 1,     // 21
         x, -y, -z,   0,  0, -1,   0, 0,     // 22
        -x, -y, -z,   0,  0, -1,   1, 0      // 23
    };

    // Vertices
    m_vertices.clear();
	S3DVertex vx= S3DVertex();
	vx.Color= m_color;
    u32 i=0;
    for( s32 j=0; j<24; j++ )
    {
        vx.Pos.set( vector3df( va[i], va[i+1], va[i+2] )); i+=3;
        vx.Normal.set( vector3df( va[i], va[i+1], va[i+2] )); i+=3;
        vx.TCoords.set( vector2df( va[i], va[i+1] )); i+=2;
        m_vertices.push_back( vx );
        //buf->Vertices.push_back( vx );
    }
	// quad indices
    m_indices.clear();
    for (u16 q=0; q<24; q++)
        m_indices.push_back( q );

    // edges
    m_edgeVertices.clear();
    m_edgeVertices= m_vertices;
    setEdgeColor( m_edgeColor );

    // edge indices
    const u16 ei[24] = { 0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,6, 1,5, 2,4, 3,7 };
    m_edgeIndices.clear();
    for (u16 e=0; e<24; e++)
        m_edgeIndices.push_back( ei[e] );
   
    m_box= aabbox3df( m_vertices[0].Pos, m_vertices[4].Pos );
}

void kxBox::setColor( const SColor& newColor )
{
    m_color= newColor;
    for( u16 v=0; v<24; v++ )
        m_vertices[v].Color= m_color;
}
void kxBox::setEdgeColor( const SColor& newColor )
{
    m_edgeColor= newColor;
    for( u16 v=0; v<24; v++ )
        m_edgeVertices[v].Color= m_edgeColor;
}



void kxBox::render()
{
	k.driver->setMaterial( m_material );
    k.driver->setTransform( ETS_WORLD, AbsoluteTransformation );
    k.driver->drawVertexPrimitiveList(
        &m_vertices[0], m_vertices.size(), 
        &m_indices[0], 
        6, // primitive count
        EVT_STANDARD, EPT_QUADS, 
        EIT_16BIT
    );

    // DRAW EDGES
    // build nudge matrix to avoid z-fighting
    vector3df camV= SceneManager->getActiveCamera()->getAbsolutePosition() - RelativeTranslation; 
    f32 length= .03f;
    f32 scale= ( camV.getLength() - length ) / camV.getLength();
    camV.setLength( length );
    camV += RelativeTranslation;
    matrix4 mx;
    mx.setScale( scale * RelativeScale );
    mx.setRotationDegrees( RelativeRotation );
    mx.setTranslation( camV );

    k.driver->setTransform( ETS_WORLD, mx );
    k.driver->setMaterial( m_edgeMat ); 
    k.driver->drawVertexPrimitiveList( &m_edgeVertices[0], m_edgeVertices.size(), &m_edgeIndices[0], 12, EVT_STANDARD, EPT_LINES );

    if( m_showNormals )
        for( u32 vx= 0; vx<m_vertices.size(); vx++ )
            k.driver->draw3DLine 
            (
                m_vertices[vx].Pos,
                m_vertices[vx].Pos + m_vertices[vx].Normal,
                SColor( 255,255,0,255 ) // purple
            );
}


void kxBox::OnRegisterSceneNode()
{
	if( IsVisible )
		SceneManager->registerNodeForRendering( this );
	ISceneNode::OnRegisterSceneNode();
}


