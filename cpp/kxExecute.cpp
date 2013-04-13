
#include "kx.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


int kx::Execute()
{
    // MAIN GAME LOOP

	while( device->run() )
	{
		if( !device->isWindowActive() ) device->yield();
            
        else 
		{
            // MAIN GAME LOOP
            time= device->getTimer()->getTime();
            fps= driver->getFPS();

            now= time;    
            deltaTime= (f64)( now - then ) / 1000.0; // in seconds
            then= now;

            // PROCESS 
            if( !isGUIvisible ) {
                switch( gameState )
                {
                    case EGS_GAME:
                    {
                        ProcessLevel();
                        ProcessPlayerState();
                        ProcessPlayerMovement();
                        // ProcessAI();
                        ProcessEntities();
                        ProcessPhysics();
                        break;
                    }
                    default: break;
                }
            }

            if( now - lastRenderTime > 7 ) // cap fps at 125
            {
                lastRenderTime= now;
                
                // RENDER
                driver->beginScene( true, true, video::SColor( 255,50,50,50 ));
                {
                    smgr->drawAll(); 
                    DrawHUD();
                    if( isGUIvisible ) DrawGUI();
                        
                    if( showDebugInfo ) DrawDebugInfo();
                    if( logDebugInfo )
                    {
                        frame++;
                        // write info to log...
                    }
                }
                driver->endScene();
            } 
		}
	}
    sound->drop();
	device->drop();

    return 0;
}

