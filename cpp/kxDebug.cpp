
#include "kxLevel.h"
#include "kxPlayerCube.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void kx::LogDebugInfo() const
{
    if( logDebugInfo ) 
    {
        stringc log;
        log += "frame";
        log += frame;
        log += " ";
        debugLog->write( log.c_str(), log.size() );
    }
}

void kx::DrawDebugInfo() const
{
    // background
    driver->draw2DRectangle
    (
        SColor( 150, 0, 0, 0),
        recti 
        (
            position2di( 0,0 ), 
            dimension2du( screenSize.Height/4, screenSize.Height/4 )
        )
    );
    // GUI visible
    stringw debugInfoString;
    debugInfoString = L"";

    debugInfoString += L"gameState=";
    switch( gameState )
    {
        case EGS_INIT:
            debugInfoString += L"INIT";
            break;
        case EGS_GAME:
            debugInfoString += L"GAME";
            break;

        default: break;
    }

    debugInfoString += L"\ntime=";
    debugInfoString += time/100;
    debugInfoString += L" ";

    debugInfoString += L"fps=";
    debugInfoString += fps;
    debugInfoString += L" ";

    if( gameState != EGS_INIT )
    {
        debugInfoString += L"\nplayerCube->moveSpeed=";
        debugInfoString += (int) playerCube->moveSpeed;
    }

    if( gameState == EGS_GAME && !isGUIvisible )
    {
        debugInfoString += L"\nplayerCube->state=";
        if (playerCube->state==EPS_CUBE)
        debugInfoString += L"CUBE";
        else if (playerCube->state==EPS_WEAPON)
        debugInfoString += L"WEAPON";
        else if (playerCube->state==EPS_DEAD)
        debugInfoString += L"DEAD";

        debugInfoString += L"\nlevel=";
        debugInfoString += activeLevel->num;
    
        debugInfoString += L"\nlevelTime=";
        debugInfoString += (int) (now - activeLevel->startTime) / 100;

        debugInfoString += L"\nprogress=";
        debugInfoString += activeLevel->progress;

        debugInfoString += L"\ncollectables=";
        debugInfoString += activeLevel->collectables.size();
    }

    if( smallFont )
    {
        smallFont->draw
        (
            debugInfoString,
            core::rect<s32>( 0,0,512,512 ),
            video::SColor( 255,255,255,255 )
        );
    }

}

void kx::VertexReport (IMesh* mesh) const
{
    // prints vertex and index info for debugging meshes
    
    SMeshBuffer* buf = (SMeshBuffer*) mesh->getMeshBuffer (0);
    u16* indices = buf->getIndices ();

    u32 vc = buf->getVertexCount();
    u32 ic = buf->getIndexCount();
    std::cout << "\tVertices:" << vc;
    std::cout << "\tIndices:" << ic << std::endl;

    u32 j=0;
    for (u32 i=0; i< ((ic/3)<vc ? vc : ic/3); ++i)
    {
        std::cout << i << ": \t";

        // vertices
        if (i<vc)
        {
            std::cout <<
            buf->Vertices[i].Pos.X << ',' <<
            buf->getPosition(i).Y << ',' << // another way to get Pos
            buf->Vertices[i].Pos.Z; 
        }
        // indices
        if (j<(ic-2))
        {
            std::cout << "\t";
            std::cout << indices[j++] << ',';
            std::cout << indices[j++] << ',';
            std::cout << indices[j++]; 
        }
        std::cout << std::endl;
    }
}



