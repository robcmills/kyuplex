#include "kxPlayerCube.h"
//#include "kxCollectable.h"
#include "kxLevel.h"
//#include "kxUserDataSerializer.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

bool kx::SaveGame()
{
    fileSys->changeWorkingDirectoryTo( kxBaseDir );

    // kxBaseDir/saves/saveName.kxSave
    stringw saveName = saveEditBox->getText();
    path filename = "./saves/";
    filename += saveName;
    filename += ".kxs";

    IWriteFile* file = fileSys->createAndWriteFile( filename );
    if (!file)
    {
        std::cout<< "Error: couldn't create save file" << endl;
        return false;
    }
    else
    {
        IXMLWriter* writer = fileSys->createXMLWriter( file );
        if (!writer)
        {
            std::cout<< "Error: couldn't create XML writer" << std::endl;
            return false;
        }
        else
        {
            writer->writeXMLHeader();

            const wchar_t* saveElement = L"kxSave";
            writer->writeElement( saveElement );
            writer->writeLineBreak();

            IAttributes* attr = fileSys->createEmptyAttributes();
            attr->addInt( "activeLevelNum", activeLevel->num );  
            attr->addInt( "activeLevelProgress", activeLevel->progress );
            // add more attrib here...

            attr->write( writer );
            attr->drop();

            writer->writeClosingTag( saveElement );
            writer->drop();
            file->drop();

            messages = ""; // reset
            messages += L"Saved "; 
            messages += saveName;
            messages += L".kxs";
            messagesNumLines= 1;
            return true;
        }
    }
    return false;
}

    //kxUserDataSerializer* kxData= new kxUserDataSerializer();
    /*
    if( !smgr->saveScene( filename, kxData ))
    {
        std::cout<< "Error: save unsuccessful" << std::endl;
        return false;
    }
    else
    {
        messages = ""; // reset
        messages += L"Saved "; 
        messages += saveName;
        messages += L".kxSave";

        return true;
    }*/


bool kx::LoadGame( const path& saveFile ) 
{
    /*
    IXMLReader* reader = fileSys->createXMLReader( saveFile );
    if (!reader)
    {
        std::cout<< "Error: couldn't create XML reader" << std::endl;
        return false;
    }
    else
    { 
        while( reader->read() )
        {
            switch( reader->getNodeType() )
            {
            case EXN_ELEMENT_END:
            std::cout<< "End reached" << std::endl;
                break;

            case EXN_ELEMENT:  // TODO: check for kxSave node?
                if( stringw( L"attributes" ) == reader->getNodeName() )
                {
                    // read attributes
                    io::IAttributes* attr = fileSys->createEmptyAttributes( driver );
                    attr->read( reader );

                    // use attributes
                    const path& activeLevelFile= attr->getAttributeAsString( "activeLevelFile" );

                    if ( !LoadLevel( activeLevelFile ))
                    {
                        std::cout<< "Error: could not initialize new game" << std::endl;
                        return false;
                    }
                    else
                    {
                        activeLevel->progress= attr->getAttributeAsInt( "activeLevelProgress" );
                        activeLevel->progress--;
                        activeLevel->startTime = now;

                        // do more loading stuff here...

                        // finished with attributes
                        attr->drop();
                        reader->drop();

                        isGUIvisible= false;
                        isFirstGUI= false;
                        gameState= EGS_GAME;
                        device->setEventReceiver( gameEventReceiver );

                        messages = ""; // reset
                        messages += L"Loaded ";
                        messages += fileSys->getFileBasename( saveFile );
                        messagesNumLines= 1;
                        return true;
                    }
                }
                break;

            default:
                break;
            }
        }
        reader->drop();
    }
    */
    return false;
}

    /*
    kxUserDataSerializer* kxData= new kxUserDataSerializer();
    if( !smgr->loadScene( fileName, kxData ))
    {
        std::cout<< "Error: load unsuccessful" << std::endl;
        LoadLevel( 1 );
        return false;
    }
    else
    {
        isGUIvisible= false;
        isFirstGUI= false;
        device->setEventReceiver( gameEventReceiver );

        messages = ""; // reset
        messages += L"Loaded ";
        messages += fileSys->getFileBasename( fileName );

        return true;
    }*/

