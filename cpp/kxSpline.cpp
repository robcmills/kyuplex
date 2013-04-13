
#include "kxSpline.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


kxSpline::kxSpline
(
    array< S3DVertex >& vertices,
    bool isOpen,
    SMaterial& mat
): 
    kxNode(),
    m_vertices( vertices ),
    m_material( mat )
{
    setName( "kxSpline" );

    if( !isOpen )
        m_vertices.push_back( vertices[0] );

    m_box= aabbox3df( m_vertices[0].Pos );

    for (u16 i=0; i<m_vertices.size(); i++)
    {
        m_indices.push_back( i );
        m_box.addInternalPoint( m_vertices[i].Pos );
    }
}

void kxSpline::OnRegisterSceneNode()
{
    if( IsVisible )
        SceneManager->registerNodeForRendering( this );

    ISceneNode::OnRegisterSceneNode();
}

void kxSpline::render()
{
    video::IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setMaterial( m_material );
    driver->setTransform( ETS_WORLD, AbsoluteTransformation );
    driver->drawVertexPrimitiveList(
        &m_vertices[0], m_vertices.size(), 
        &m_indices[0], 
        m_vertices.size()-1, // primitive count
        video::EVT_STANDARD, scene::EPT_LINE_STRIP, 
        video::EIT_16BIT
    );
}

