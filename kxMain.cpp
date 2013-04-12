#include "kx.h"
#include "kxGUIEventReceiver.h"
#include "kxGameEventReceiver.h"

int main()
{
    // SINGLETON DESIGN
    kx& kyuplex= kx::getInstance();

    if( kyuplex.Init() )
    {
        std::cout<< "Initialization failed" << std::endl;
        return 0; 
    }

    kxGUIEventReceiver GUIReceiver( kx::getInstance() );
    kyuplex.GUIEventReceiver= &GUIReceiver;
    kyuplex.device->setEventReceiver( &GUIReceiver );

    kxGameEventReceiver gameReceiver( kx::getInstance() );
    kyuplex.gameEventReceiver= &gameReceiver;

    return kyuplex.Execute();
}
