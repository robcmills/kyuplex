#ifndef __KX_LIGHT_H_INCLUDED__
#define __KX_LIGHT_H_INCLUDED__

#include "kxNode.h"
//#include "kxEnums.h"
//#include <IMeshSceneNode.h>
//#include <SMesh.h>

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/** kxLight's are dynamic with a btRigidbody and btSphereShape
 *  can be maniuplated by player
 *  rendered with a billboard
 */
class kxLight : public kxNode
{
public:
    //! constructor
    kxLight();
    virtual ~kxLight(); 

    //virtual void setSize( const f32 newSize ); 
    //f32 getSize() { return m_size; }

    virtual void setPosition( const btVector3& pos ); 


    bool m_showRays;
    f32 m_radius, m_bbSize;

    //SMaterial m_material;
    //SMaterial m_edgeMat;

    ILightSceneNode* lightNode;
    IBillboardSceneNode* bb;
    btRigidBody* rb;
    btScalar defLinDamp, defAngDamp;
};


#endif
