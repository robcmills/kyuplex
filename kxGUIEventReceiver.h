#ifndef __KX_GUI_EVENTRECEIVER_H_INCLUDED__
#define __KX_GUI_EVENTRECEIVER_H_INCLUDED__

//#include "kx.h"
#include "kxPlayerCube.h" // <<?
#include "kxLevel.h" // <<?

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



class kxGUIEventReceiver : public IEventReceiver
{
public:
	kxGUIEventReceiver( kx& context ) : k( context ) { }

	virtual bool OnEvent( const SEvent& event )
	{
		if (event.EventType == EET_GUI_EVENT && k.isGUIvisible)
		{
			s32 id = event.GUIEvent.Caller->getID();

			switch (event.GUIEvent.EventType)
			{

            case EGET_FILE_SELECTED:
                if( id == EGID_LOAD_DIALOG )
                {
                    k.fileSys->changeWorkingDirectoryTo( k.kxBaseDir );
                    k.GUI->setFocus( k.titleWindow );
                    k.sound->play2D( k.triA3 );

                    const path& fileName = k.fileOpenDialog->getFileName(); 

                    if( k.beginWindow->isVisible() ) 
                    {
                        k.beginWindow->setVisible( false );
                        if( !k.LoadGame( fileName ))
                        {
                            k.messages= "";
                            k.messages += L"Error: could not load saved game";
                            k.messagesNumLines= 1;
                        }
                        else
                        {
                            k.gameState= EGS_GAME;
                            k.device->setEventReceiver( k.gameEventReceiver );
                        }
                        return true;
                    }
                }
                return true;
            case EGET_FILE_CHOOSE_DIALOG_CANCELLED:
                k.fileSys->changeWorkingDirectoryTo( k.kxBaseDir );
                k.sound->play2D( k.triG2 );         // TODO: why is this not working?
                k.GUI->setFocus( k.beginWindow ); 
                return true;


			case EGET_BUTTON_CLICKED: // {{{1
				switch (id)
				{

                // TITLE MENU {{{2
				case EGID_BEGIN_BUTTON:
                    k.sound->play2D( k.triC3 );
                    k.beginWindow->setVisible( true );
                    k.GUI->setFocus( k.beginWindow );
					return true;
	
				case EGID_SETTINGS_BUTTON:
                    k.sound->play2D( k.triA3 );
                        k.messages = ""; // reset
                        k.messages += L"Under construction. Please try again later.";
                        k.messagesNumLines= 1;
                    //k.settingsWindow->setVisible( true );
				    //k.GUI->setFocus( k.settingsWindow );	
					return true;

				case EGID_END_BUTTON:
                    k.sound->play2D( k.scaleDown );
                    while (k.sound->isCurrentlyPlaying( k.scaleDown )) {};
                    // wait for sound to finish playing before quitting
					k.device->closeDevice();
					return true;
                // end TITLE MENU }}}

                // BEGIN WINDOW {{{2
                case EGID_BEGIN_NEW_BUTTON:
                    k.sound->play2D( k.triE3 );
                    if( !k.isFirstGUI ) return true;

                    k.beginWindow->setVisible( false );
                    k.GUI->setFocus( k.titleWindow );

                    k.messages = ""; // reset
                    k.messagesNumLines= 0;

                    k.BeginNewGame();
                    return true;

                case EGID_BEGIN_LOAD_BUTTON:
                    {
                        k.sound->play2D (k.triA3);
                            k.messages = ""; // reset
                            k.messages += L"Under construction. Please try again later.";
                            k.messagesNumLines= 1;
                        /*

                        path saveDir= k.kxBaseDir;
                        saveDir += "/saves/";
                        if (k.fileSys->changeWorkingDirectoryTo( saveDir ))
                        {
                            k.fileOpenDialog = k.GUI->addFileOpenDialog (L"Load", true, k.beginWindow); 
                            if (k.fileOpenDialog) 
                            {
                                k.fileOpenDialog->setRelativePosition
                                (
                                    rect<s32>
                                    (
                                        position2di( 0, k.buttonSize ),
                                        dimension2di( k.buttonSize*k.numButtons, k.buttonSize*2 )
                                    )
                                );
                                k.fileOpenDialog->setID (EGID_LOAD_DIALOG);
                                k.fileOpenDialog->setSubElement ( true );
                                return true;
                            }
                        }
                        else std::cout<< "Error: couldn't change dir in kxGUIEventReceiver.h case EGID_BEGIN_LOAD_BUTTON" << std::endl;
                        */
                    }
                    return true;

                case EGID_BEGIN_SAVE_BUTTON:
                    /*
                    if (!k.isFirstGUI) 
                    {
                        k.sound->play2D( k.triB3 );
                        k.saveWindow->setVisible( true );
                        k.GUI->setFocus( k.saveWindow );
                        k.saveEditBox->setText( L"saveFileName" );
                    }
                    else 
                    */
                    {
                        k.sound->play2D( k.triG2 );
                        k.messages = ""; // reset
                        //k.messages += L"Scene is empty! Begin a game.";
                        k.messages += L"Under construction. Please try again later.";
                        k.messagesNumLines= 1;
                    }
                    return true;

                case EGID_BEGIN_BACK_BUTTON:
                    k.sound->play2D( k.triG2 );
                    k.beginWindow->setVisible( false );
                    k.GUI->setFocus( k.titleWindow );
                    return true;

                
                // end BEGIN WINDOW }}}

                // SETTINGS WINDOW {{{2
                case EGID_SETTINGS_PHYSICS_BUTTON:
                    k.sound->play2D( k.triD3 );
                    k.messages= "";
                    k.messages= "Under construction. Please try again later.";
                    k.messagesNumLines= 1;
                    //k.physicsWindow->setVisible( true );
				    //k.GUI->setFocus( k.physicsWindow );	
                    return true;

                case EGID_SETTINGS_BACK_BUTTON:
                    k.sound->play2D( k.triG2 );
                    k.settingsWindow->setVisible( false );
                    k.GUI->setFocus( k.titleWindow );
                    return true;
                // end SETTINGS WINDOW }}}

                //  PHYSICS WINDOW {{{2
                case EGID_PHYSICS_BACK_BUTTON:
                    k.sound->play2D( k.triG2 );
                    k.physicsWindow->setVisible( false );
                    k.GUI->setFocus( k.settingsWindow );
                    return true;
                // end PHYSICS WINDOW }}}

                // SAVE WINDOW {{{2
                case EGID_SAVE_OK_BUTTON:
                    k.sound->play2D( k.triF3 );
                    k.saveWindow->setVisible( false ); 
                    k.GUI->setFocus( k.titleWindow );
                    //k.isGUIvisible=false;
                    k.cursor->setPosition( k.screenCenter );

                    if( k.beginWindow->isVisible() ) 
                    {
                        k.beginWindow->setVisible( false );
                        k.SaveGame();
                        return true;
                    }

                case EGID_SAVE_CANCEL_BUTTON:
                    k.sound->play2D( k.triG2 );
                    k.saveWindow->setVisible( false ); 
                    //k.beginWindow->setVisible( true );
                    k.GUI->setFocus( k.beginWindow );
                    return true;

                // end SAVE WINDOW }}}

                // LEVEL PROGRESS MODALS
                case EGID_LVL_PROGRESS_OK_BUTTON:
                    k.sound->play2D( k.triF3 );
                    event.GUIEvent.Caller->getParent()->remove();
                    k.titleWindow->setVisible( true );
                    k.GUI->setFocus( k.titleWindow );
                    k.isGUIvisible=false;
                    k.device->setEventReceiver( k.gameEventReceiver );
                    k.cursor->setPosition( k.screenCenter );
                    return true;


				default:
					return false;
				} 
				break;
                // }}} end EGET_BUTTON_CLICKED


            /*
            case EGET_SPINBOX_CHANGED:
                switch (id)
                {
                    case EGID_PHYSICS_WORLD_GRAV_X:
                        k.physicsGrav.setX( k.worldGravXBox->getValue() );
                        k.world->setGravity( k.worldGrav );
                        return true;
                    case EGID_PHYSICS_WORLD_GRAV_Y:
                        k.worldGrav.setY( k.worldGravYBox->getValue() );
                        k.physics->setGravity( k.worldGrav );
                        return true;
                    case EGID_PHYSICS_WORLD_GRAV_Z:
                        k.worldGrav.setZ( k.worldGravYBox->getValue() );
                        k.physics->setGravity( k.worldGrav );
                        return true;

                    case EGID_PHYSICS_TCUBE_REST_BOX:
                        if (!k.isFirstGUI)
                            //k.tCube1RigidBody->setRestitution (k.tCubeRestBox->getValue());
                        return true;

                    default:
                        return false;
                }
                break;
                */

			default:
				break;
			} 
            
		} // END GUI EVENTS



        // RELEASED KEYS
		if (event.EventType == EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) 
		{
            switch (event.KeyInput.Key)
            {
            case KEY_ESCAPE:
                if( !k.isFirstGUI ) // can't hide menu if game hasn't started
                {
                    k.isGUIvisible = false; 
                    //k.titleWindow->setVisible( false );
                    //k.toolbarWin->setVisible( true );
                    k.cursor->setPosition( k.screenCenter );
                    if( k.gameState == EGS_GAME )
                        k.device->setEventReceiver( k.gameEventReceiver );
                }
                return true;

            case KEY_RETURN:
                if( k.saveWindow->isVisible() ) // return key presses ok button
                {
                    SEvent okEvent;
                    okEvent.EventType= EET_GUI_EVENT;
                    okEvent.GUIEvent.Caller= k.saveOkButton;
                    okEvent.GUIEvent.Element= k.saveOkButton;
                    okEvent.GUIEvent.EventType= EGET_BUTTON_CLICKED;
                    k.saveOkButton->OnEvent( okEvent );
                }
                return true;

            //case KEY_TAB:
                //return true;
                

            default:
                break;
            }

        } // END KEYS

		return false;
	}
private:
    kx& k;
};


#endif
