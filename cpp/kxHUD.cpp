
//#include "kx.h"
#include "kxLevel.h"
#include "kxPlayerCube.h"

//using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void kx::DrawHUD() const
{
    driver->setTransform( ETS_WORLD, matrix4() );
    driver->setMaterial( whiteMat );

    // messages background
    driver->draw2DRectangle
    (
        SColor( 150, 0, 0, 0),
        recti 
        (
            position2di( 0, screenSize.Height - 18 * messagesNumLines ), 
            dimension2du( screenSize.Width, 64 )
        )
    );

    // messages
    if( smallFont )
        smallFont->draw
        (
            messages,
            rect<s32> (8, screenSize.Height-16*messagesNumLines, 512, 64),
            SColor(255,255,255,255)
        );


    if( gameState == EGS_GAME )
    {
        //DrawProgressBars();
        //DrawGrid();
        //DrawAxii( playerCube->getPosition(), 1.6f ); 

        // INTEGRITY
        stringw integrityString;
        integrityString += playerCube->integrity;
        largeFont->draw
        (
            integrityString,
            core::rect< s32>( screenCenter.X+buttonSize*2, screenCenter.Y+buttonSize*2, 512, 512),
            video::SColor( 255, 255, 255, 255 )
        );

        // TOOLS
        if( playerCube->hasSoundTool ) {
            driver->draw2DImage(
                soundToolTex, 
                core::position2di( screenCenter.X, screenCenter.Y+buttonSize*2 ), // destPos
                core::rect< s32 >( position2di(0,0), dimension2di(32,32) ), // sourceRect
                0, // clip rect 
                SColor(255, 255, 255, 255), 
                true ); // useAlphaChannelOfTexture
            if( playerCube->activeTool == KT_SOUND ) {
                driver->draw2DRectangleOutline(
                    recti( 
                        position2di( screenCenter.X, screenCenter.Y+buttonSize*2 ), 
                        dimension2du( 32,32 )),
                    SColor( 255,255,255,255 ));
            }

        }

    }

    // debug
    if( showLights ) DrawLights();

    if( showPhysicsWireframes ) {
        physics->getDebugDrawer()->setDebugMode (btIDebugDraw::DBG_DrawWireframe);
        physics->debugDrawWorld(); // physics wireframes, rigid bodies, etc.
        physics->getDebugDrawer()->setDebugMode (btIDebugDraw::DBG_DrawConstraints);
        physics->debugDrawWorld(); }



}
void kx::DrawLights() const
{
    /*
    if( gameState != EGS_GAME ) return;

    driver->setTransform( ETS_WORLD, matrix4() );
    driver->setMaterial( whiteMat );

    for( u32 l=0; l<activeLevel->lights.size(); l++ ) 
    {
        vector3df origin( activeLevel->lights[l]->getPosition() );
        f32 length( activeLevel->lights[l]->getRadius() );
        SColor color( 255,255,255,0 ); // yellow

        driver->draw3DLine // x axis
        (
            origin - vector3df( length,0,0 ),
            origin + vector3df( length,0,0 ),
            color
        );
        driver->draw3DLine // y axis
        (
            origin - vector3df( 0,length,0 ),
            origin + vector3df( 0,length,0 ),
            color
        );
        driver->draw3DLine // z axis
        (
            origin - vector3df( 0,0,length ),
            origin + vector3df( 0,0,length ),
            color
        );
    }
    */

}


void kx::DrawProgressBars() const
{
    /*
    kxLevel* lvl= activeLevel;

    if( lvl->progress != 0 ) 
    {
        // LEVEL
        f32 lvlProgress= 0;
        lvlProgress= f32(lvl->progress - 2) / f32(lvl->progressTotal - 2);

        // background
        driver->draw2DRectangle
        (
            SColor( 150, 0, 0, 0 ),
            recti 
            (
                position2di( screenSize.Width - buttonSize*2, buttonSize/4 ), 
                dimension2du( buttonSize, buttonSize/8 )
            )
        );
        // green bar
        driver->draw2DRectangle
        (
            SColor( 150, 0, 255, 0 ),
            recti 
            (
                position2di( screenSize.Width - buttonSize*2, buttonSize/4 ), 
                dimension2du( u32(buttonSize * lvlProgress), buttonSize/8 )
            )
        );
        // outline
        driver->draw2DRectangleOutline 
        (
            recti 
            (
                position2di( screenSize.Width - buttonSize*2, buttonSize/4 ), 
                dimension2du( buttonSize, buttonSize/8 )
            )
        );

        // OBJECTIVES
        u32 obj= 0;
        if( lvl->objectives != 0 )
            obj= u32( buttonSize - f32(activeLevel->numActiveCollectables) / f32(activeLevel->objectives) * buttonSize ); 

        // background
        driver->draw2DRectangle
        (
            SColor( 150, 0, 0, 0 ),
            recti 
            (
                position2di( screenSize.Width - buttonSize*2, buttonSize/2 ), 
                dimension2du( buttonSize, buttonSize/8 )
            )
        );
        driver->draw2DRectangle
        (
            SColor( 150, 0, 255, 0 ),
            recti 
            (
                position2di( screenSize.Width - buttonSize*2, buttonSize/2 ), 
                dimension2du( obj, buttonSize/8 )
            )
        );
        driver->draw2DRectangleOutline 
        (
            recti 
            (
                position2di( screenSize.Width - buttonSize*2, buttonSize/2 ), 
                dimension2du( buttonSize, buttonSize/8 )
            )
        );
    }
    */
}


void kx::DrawGrid() const
{
    driver->setMaterial( whiteMat );
    matrix4 m;
    driver->setTransform( ETS_WORLD, m );

    // TODO: convert to SceneNodes


    // GRID
    for (f32 z=-25.6f; z<25.7f; z+=6.4f)
    {
        driver->draw3DLine
        (
            vector3df (25.6f,0.f,z), 
            vector3df (-25.6f,0.f,z),
            video::SColor (55,155,155,155)
        );
    }
        
    for (f32 y=-25.6f; y<25.7f; y+=6.4f)
    {
        driver->draw3DLine
        (
            vector3df (y,0.f,25.6f), 
            vector3df (y,0.f,-25.6f),
            video::SColor (55,155,155,155)
        );
    }

    /*
    {
        vector3df cPos = camPivot->getAbsolutePosition() - playerCam->getAbsolutePosition();
        cPos.normalize();
        vector3df strafe= cPos;
        strafe.Y= 0;
        strafe.setLength( 3.2f );
        strafe.rotateXZBy( 90, vector3df( 0 ));
        vector3df upv= strafe.crossProduct( cPos );
        //upv.Y *= -1;
        upv.setLength( 3.2f );

        driver->draw3DBox
        (
            aabbox3df(
                playerCube->getAbsolutePosition(),
                playerCube->getAbsolutePosition() + upv ),
            video::SColor (255,255,255,0)
        );
    }
    */

}

void kx::DrawCollectableLines() const
{
    /*
    for( u32 i=0; i<activeLevel->collectables.size(); i++ )
    {
        if( i == activeLevel->collectables.size() )
        */

}

void kx::DrawAxii( const vector3df& origin, const f32 length ) const
{
    // TODO: convert to SceneNode?

    driver->draw3DLine // x axis
    (
        origin,
        origin + vector3df( length,0,0 ),
        video::SColor (255,255,0,0) // red
    );
    driver->draw3DLine // y axis
    (
        origin,
        origin + vector3df( 0,length,0 ),
        video::SColor (255,0,255,0) // green
    );
    driver->draw3DLine // z axis
    (
        origin,
        origin + vector3df( 0,0,length ),
        video::SColor (255,0,0,255) // blue
    );

}
