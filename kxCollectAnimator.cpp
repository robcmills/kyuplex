
#include "kxCollectAnimator.h"
#include "kxPlayerCube.h"
//#include "ITexture.h"

//namespace irr
//{
//namespace scene
//{


//! constructor
kxCollectAnimator::kxCollectAnimator( u32 now )
:
    k( kx::getInstance() ),
	m_timePerFrame( 100 ), m_startTime( now )
{
	#ifdef _DEBUG
	setDebugName("kxCollectAnimator");
	#endif

    m_textures.push_back( k.greenTex );
    m_textures.push_back( k.playerTex );
    m_textures.push_back( k.greenTex );
    m_textures.push_back( k.playerTex );

	m_finishTime = now + ( m_timePerFrame * m_textures.size() );
}

//! destructor
kxCollectAnimator::~kxCollectAnimator()
{
    m_textures.clear();
	//clearTextures();
}

/*
void kxCollectAnimator::clearTextures()
{
	for( u32 i=0; i<m_textures.size(); ++i )
		if( m_textures[i] )
			m_textures[i]->drop();
}
*/


void kxCollectAnimator::animateNode( ISceneNode* node, u32 timeMs )
{
	if(!node || node->getID() != KNT_PLAYER_CUBE )
		return;

    const u32 t = ( timeMs - m_startTime );

    u32 idx = 0;
    if( timeMs >= m_finishTime )
    {
        idx= m_textures.size() - 1;
        m_isFinished= true;
    }
    else
    {
        idx= ( t/m_timePerFrame ) % m_textures.size();
    }

    if( idx < m_textures.size() )
    {
        node->setMaterialTexture( 0, m_textures[idx] );

        if( idx == 0 || idx == 2 )
            node->getMaterial(0).Lighting= false;
        else
            node->getMaterial(0).Lighting= true;
    }
}


//} // end namespace scene
//} // end namespace irr

