
#include "kxPlayerCube.h"
#include "kxLevel.h"
#include "kxCollectable.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


void kx::ProcessLevel()
{
    //activeLevel->num= 5;

    switch( activeLevel->num )
    {
    //case 0: ProcessEditor(); break;
    case 1: ProcessLevel1(); break;
    case 2: ProcessLevel2(); break;
    case 3: ProcessLevel3(); break;
    case 4: ProcessLevel4(); break;
    case 5: ProcessLevel5(); break;

    default: break;
    }
}

void kx::ProcessLevel1() // {{{
{
    kxLevel* lvl = activeLevel;
    switch( lvl->progress )
    {
        case 0:
        {
            InitModal 
            (
                L" Level 1", 
                L" -Use coplanar movement WASD \n -Collect collectables"
            );
            lvl->progress++;
        }
        break;

        case 1:
        {
            LoadEntities();
            lvl->progressTotal= 8;

            physics->setGravity( btVector3( 0,0,0 ));

            lvl->startTime = now;
            lvl->progress++;
        }
        break;

        case 6:
            if( lvl->collectables.size() == 0 )
            {
                stringw doneInfo( L"" );
                doneInfo += L" Time to complete: ";
                doneInfo += (int) (now - activeLevel->startTime) / 100;
                InitModal 
                (
                    L" Level 1 completed.", 
                    doneInfo
                );
                lvl->progress++;
            }
            break;

        case 7: 
            playerCube->reset();
            activeLevel->reset();
            lvl->num++;
            lvl->progress= 0;
            break;

        case 8: 
            break;

        default: 
            if( lvl->collectables.size() == 0 )
            {
                LoadEntities();
                lvl->progress++;
            }
            break;

    }
} // }}}

void kx::ProcessLevel2() // {{{
{
    kxLevel* lvl = activeLevel;
    switch( lvl->progress )
    {
        case 0:
        {
            InitModal 
            (
                L" Level 2", 
                L" -Use vertical movement: \n Spacebar=Up Shift=Down"
            );
            lvl->progress++;
        }
        break;

        case 1:
        {
            LoadEntities();
            lvl->progressTotal= 6;

            physics->setGravity( btVector3( 0,0,0 ));

            lvl->startTime = now;
            lvl->progress++;
        }
        break;

        case 5:
            if( lvl->collectables.size() == 0 )
            {
                stringw doneInfo( L"" );
                doneInfo += L" Time to complete: ";
                doneInfo += (int) (now - activeLevel->startTime) / 100;
                InitModal 
                (
                    L" Level 2 completed.", 
                    doneInfo
                );
                lvl->progress++;
            }
            break;

        case 6:
            playerCube->reset();
            activeLevel->reset();
            lvl->num++;
            lvl->progress= 0;
            break;


        default: 
            if( lvl->collectables.size() == 0 )
            {
                LoadEntities();
                lvl->progress++;
            }
            break;

    }
} // }}}

void kx::ProcessLevel3() // {{{
{
    kxLevel* lvl = activeLevel;
    switch( lvl->progress )
    {
        case 0:
        {
            InitModal 
            (
                L" Level 3", 
                L" -Rigid cubes"
            );
            lvl->progress++;
        }
        break;

        case 1:
        {
            LoadEntities();
            lvl->progressTotal= 6;

            physics->setGravity( btVector3( 0,0,0 ));

            lvl->startTime = now;
            lvl->progress++;
        }
        break;

        case 5:
            if( lvl->collectables.size() == 0 )
            {
                stringw doneInfo( L"" );
                doneInfo += L" Time to complete: ";
                doneInfo += (int) (now - activeLevel->startTime) / 100;
                InitModal 
                (
                    L" Level 3 completed.", 
                    doneInfo
                );
                lvl->progress++;
            }
            break;

        case 6:
            playerCube->reset();
            activeLevel->reset();
            lvl->num++;
            lvl->progress= 0;
            break;


        default: 
            if( lvl->collectables.size() == 0 )
            {
                LoadEntities();
                lvl->progress++;
            }
            break;

    }
} // }}}

void kx::ProcessLevel4() // {{{
{
    kxLevel* lvl = activeLevel;
    switch( lvl->progress )
    {
        case 0:
        {
            InitModal 
            (
                L" Level 4", 
                L" -Arcs & Circles & Kill Cubes"
            );
            lvl->progress++;
        }
        break;

        case 1:
        {
            LoadEntities();
            lvl->progressTotal= 6;

            physics->setGravity( btVector3( 0,0,0 ));

            lvl->startTime = now;
            lvl->progress++;
        }
        break;

        case 5:
            if( lvl->collectables.size() == 0 )
            {
                stringw doneInfo( L"" );
                doneInfo += L" Time to complete: ";
                doneInfo += (int) (now - activeLevel->startTime) / 100;
                InitModal 
                (
                    L" Level 4 completed.", 
                    doneInfo
                );
                lvl->progress++;
            }
            break;

        case 6:
            playerCube->reset();
            activeLevel->reset();
            lvl->num++;
            lvl->progress= 0;
            break;


        default: 
            if( lvl->collectables.size() == 0 )
            {
                LoadEntities();
                lvl->progress++;
            }
            break;

    }
} // }}}

void kx::ProcessLevel5() // {{{
{
    kxLevel* lvl = activeLevel;
    switch( lvl->progress )
    {
        case 0:
        {
            InitModal 
            (
                L" Lesson 5", 
                L" Acquire [[sound tool]]\n Press 'F' key to equip\n Click left mouse to use"
            );
            lvl->progress++;
        }
        break;

        case 1:
        {
            LoadEntities();
            lvl->progressTotal= 6;

            physics->setGravity( btVector3( 0,0,0 ));

            lvl->startTime = now;
            lvl->progress++;
        }
        break;

        case 5:
            if( lvl->collectables.size() == 0 )
            {
                stringw doneInfo( L"" );
                doneInfo += L" Time to complete: ";
                doneInfo += (int) (now - activeLevel->startTime) / 100;
                InitModal 
                (
                    L" Lesson 5 completed.", 
                    doneInfo
                );
                lvl->progress++;
            }
            break;

        case 6:
            playerCube->reset();
            activeLevel->reset();
            lvl->num++;
            lvl->progress= 0;
            break;


        default: 
            if( lvl->collectables.size() == 0 )
            {
                LoadEntities();
                lvl->progress++;
            }
            break;

    }
} // }}}
