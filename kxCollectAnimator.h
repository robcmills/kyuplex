
#ifndef __KX_COLLECT_ANIMATOR_H_INCLUDED__
#define __KX_COLLECT_ANIMATOR_H_INCLUDED__

#include <irrlicht.h>
#include "kx.h"
//#include "ISceneNodeAnimatorFinishing.h"

//namespace irr
//{
//namespace scene
//{

class kxCollectAnimator : public ISceneNodeAnimator
{
public:

    kxCollectAnimator( u32 now ); 
    virtual ~kxCollectAnimator();

    virtual void animateNode( ISceneNode* node, u32 timeMs );

    virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_TEXTURE; }
   
    virtual bool hasFinished( void ) const { return m_isFinished; }

    virtual ISceneNodeAnimator* createClone( ISceneNode* node, ISceneManager* newManager ) { return 0; }
private:

    //void clearTextures();

    core::array<video::ITexture*> m_textures;
    u32 m_timePerFrame;
    u32 m_startTime, m_finishTime;
    bool m_isFinished;

    kx& k;
};


//} // end namespace scene
//} // end namespace irr

#endif

