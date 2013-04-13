
#include "kx.h"

using namespace irr;

class kxDebugDrawer : public btIDebugDraw
{
	video::IVideoDriver* m_driver;
	int m_debugMode;

public:
	kxDebugDrawer (video::IVideoDriver* driver):
        m_driver (driver), m_debugMode (0) {}

	///most important debug drawing method, drawing lines
	virtual void drawLine (const btVector3& from, const btVector3& to, const btVector3& color)
	{
        video::SMaterial mat= video::SMaterial();
        mat.setFlag( video::EMF_LIGHTING, false );
        m_driver->setMaterial( mat );
		m_driver->draw3DLine(( const irr::core::vector3df&) from, (const irr::core::vector3df&) to, irr::video::SColor (255,247,212,0));
        // orange color for debug lines
	}

	///optional debug methods
	virtual void drawContactPoint (const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) 
    {
        video::SMaterial mat= video::SMaterial();
        mat.setFlag( video::EMF_LIGHTING, false );
        m_driver->setMaterial( mat );
        m_driver->draw3DLine ((const irr::core::vector3df&) pointOnB, (const irr::core::vector3df&) normalOnB, irr::video::SColor (255,247,212,0));
    }
	virtual void reportErrorWarning (const char* warningString) {}
	virtual void draw3dText (const btVector3& location, const char* textString) {}

	virtual void setDebugMode (int debugMode) {m_debugMode = debugMode;}
	virtual int	 getDebugMode() const {return m_debugMode;}
};

