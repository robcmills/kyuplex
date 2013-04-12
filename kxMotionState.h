#ifndef __KX_MOTIONSTATE_H_INCLUDED__
#define __KX_MOTIONSTATE_H_INCLUDED__

//#include "kx.h"
#include "kxPlayerCube.h"
//#include "kxCube.h"

///This class sychronizes the world transform between Bullet rigid bodies and their accompanying Irrlicht nodes
class kxMotionState : public btDefaultMotionState
{
	ISceneNode* m_node;
    kx& k;      // context

public:
	kxMotionState( ISceneNode* node )
		:m_node( node ), k( kx::getInstance() ) {}

	///synchronizes world transform from user to physics
	virtual void getWorldTransform( btTransform& centerOfMassWorldTrans ) const 
	{
		centerOfMassWorldTrans = m_graphicsWorldTrans;
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform( const btTransform& centerOfMassWorldTrans )
	{
		m_startWorldTrans= centerOfMassWorldTrans;
		m_graphicsWorldTrans= centerOfMassWorldTrans;

        // newPos
		const btVector3& Point = centerOfMassWorldTrans.getOrigin();
        vector3df newPos = vector3df( (f32)Point[0], (f32)Point[1], (f32)Point[2] ); /// k->physicsWorldScaling);
        m_node->setPosition( newPos );

        // newRot
        btVector3 euler;
        k.QuaternionToEuler( centerOfMassWorldTrans.getRotation(), euler );
        vector3df newRot= vector3df( euler[0], euler[1], euler[2] );

        // prevent gimbal lock
        if( m_node->getID() == KNT_PLAYER_CUBE && k.gameState == EGS_GAME ) 
        {
            vector3df cRot= k.camPivot->getRotation();
            vector3df newAbsRot= newRot + cRot;

            if( newAbsRot.X > 89.9f )
                cRot.X -= ( newAbsRot.X - 89.9f );
            if( newAbsRot.X < -89.9f )
                cRot.X -= ( newAbsRot.X + 89.9f );

            if( newRot.Z + cRot.X > 89.9f )
                cRot.X -= ( newRot.Z + cRot.X - 89.9f );
            if( newRot.Z + cRot.X < -89.9f )
                cRot.X -= ( newRot.Z + cRot.X + 89.9f );

            k.camPivot->setRotation( cRot );
        }
        m_node->setRotation( newRot );
	}
};

#endif
