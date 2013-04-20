
#include "kx.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void kx::InitGUI ()
{
    GUI = device->getGUIEnvironment();

    largeFont = GUI->getFont("./fonts/monospaceFontLarge.png");
    smallFont = GUI->getFont("./fonts/monospaceFontSmall.png");

    guiSkin = GUI->createSkin (gui::EGST_WINDOWS_CLASSIC);

    guiSkin->setFont (smallFont);
    guiSkin->setColor (gui::EGDC_BUTTON_TEXT, video::SColor (255,255,255,255));
    guiSkin->setColor (gui::EGDC_3D_FACE, video::SColor (255,40,40,40));
    guiSkin->setColor (gui::EGDC_3D_DARK_SHADOW, video::SColor (255,20,20,20));
    guiSkin->setColor (gui::EGDC_3D_HIGH_LIGHT, video::SColor (255,60,60,60));
    guiSkin->setColor (gui::EGDC_3D_SHADOW, video::SColor (255,40,40,40));
    guiSkin->setColor (gui::EGDC_ACTIVE_BORDER, video::SColor (255,100,200,100));
    guiSkin->setColor (gui::EGDC_EDITABLE, video::SColor (255,100,100,100));
    guiSkin->setColor (gui::EGDC_FOCUSED_EDITABLE, video::SColor (255,80,80,80));

    GUI->setSkin (guiSkin);
    

    // BUTTONS
    numButtons = 4;
    buttonSize = 128;
    winPos = core::position2di (screenSize.Width/2 - numButtons/2*buttonSize, screenSize.Height/2 - numButtons/2*buttonSize);


    /* GUI code is lengthy, so marker folding is used heavily here;
     * http://vimdoc.sourceforge.net/htmldoc/fold.html#folding 
     */

    
    titleWindow = GUI->addWindow // {{{1
    (
        core::rect<s32>
        (
            winPos,
            core::dimension2di (buttonSize*numButtons, buttonSize*numButtons)
        ),
        false,  // modal?
        L"kyuplex", // titlebar text
        GUI->getRootGUIElement (),  // parent
        EGID_TITLE_WINDOW // id
    );
    //titleWindow->setDraggable (false);
    titleWindow->getCloseButton()->setVisible (false);
    //titleWindow->setDrawTitlebar (false);
    //GUI->setFocus (titleWindow);


    core::position2di buttonPos = titleWindow->getClientRect().UpperLeftCorner;
    buttonPos += position2di (2,2);

    beginButton = GUI->addButton // {{{2
    (
        rect<s32>
        (
            position2di (buttonSize, buttonSize),
            core::dimension2di (buttonSize, buttonSize)
        ),
        titleWindow,
        EGID_BEGIN_BUTTON, // id
        L"Begin" // button text
        //, L"Tooltip text"
    );
    beginButton->setSubElement (true);
    beginButton->setTabStop (false);
    beginButton->setPressedImage 
    (
        greenTex, 
        rect<s32>
        (
            position2di (0,0),
            dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    settingsButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            position2di (buttonSize*2, buttonSize),
            dimension2di (buttonSize, buttonSize)
        ),
        titleWindow,
        EGID_SETTINGS_BUTTON, // id
        L"Settings" // button text
        //, L"Tooltip text"
    );
    settingsButton->setSubElement (true);
    settingsButton->setTabStop (false);
    settingsButton->setPressedImage 
    (
        orangeTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    editorButton = GUI->addButton // {{{2
        // TODO: change to help
    (
        core::rect<s32>
        (
            position2di (buttonSize, buttonSize*2),
            dimension2di (buttonSize, buttonSize)
        ),
        titleWindow,
        EGID_EDIT_BUTTON, // id
        L"?" // button text
        //, L"Tooltip text"
    );
    editorButton->setSubElement (true);
    editorButton->setTabStop (false);
    editorButton->setPressedImage 
    (
        purpleTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    endButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            core::position2di (buttonSize*3, buttonSize*3),
            core::dimension2di (buttonSize, buttonSize)
        ),
        titleWindow,
        EGID_END_BUTTON, // id
        L"End" // button text
        //, L"Tooltip text"
    );
    endButton->setSubElement (true);
    endButton->setTabStop (false);
    endButton->setPressedImage 
    (
        redTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    // end titleWindow }}}

    beginWindow = GUI->addWindow // {{{1
    (
        core::rect<s32>
        (
            position2di (0,0),
            core::dimension2di (buttonSize*numButtons, buttonSize*numButtons)
        ),
        false,  // modal?
        L"Begin", // titlebar text
        titleWindow, // parent
        EGID_BEGIN_WINDOW // id
    );
    beginWindow->getCloseButton()->setVisible (false);
    beginWindow->setVisible (false);
    beginWindow->setDraggable (false);
    beginWindow->setSubElement (true);

    beginNewButton = GUI->addButton // {{{2
    (
        rect<s32>
        (
            position2di (buttonSize, buttonSize),
            core::dimension2di (buttonSize, buttonSize)
        ),
        beginWindow,
        EGID_BEGIN_NEW_BUTTON, // id
        L"New" // button text
        // L"Tooltip text"
    );
    beginNewButton->setSubElement (true);
    beginNewButton->setTabStop (false);
    beginNewButton->setPressedImage 
    (
        greenTex, 
        rect<s32>
        (
            position2di (0,0),
            dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    beginLoadButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            position2di (buttonSize*2, buttonSize),
            dimension2di (buttonSize, buttonSize)
        ),
        beginWindow,
        EGID_BEGIN_LOAD_BUTTON, // id
        L"Load" // button text
        //, L"Tooltip text"
    );
    beginLoadButton->setSubElement (true);
    beginLoadButton->setTabStop (false);
    beginLoadButton->setPressedImage 
    (
        purpleTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    beginSaveButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            core::position2di (buttonSize*2, buttonSize*2),
            core::dimension2di (buttonSize, buttonSize)
        ),
        beginWindow,
        EGID_BEGIN_SAVE_BUTTON, // id
        L"Save" // button text
        //, L"Tooltip text"
    );
    beginSaveButton->setSubElement (true);
    beginSaveButton->setTabStop (false);
    beginSaveButton->setPressedImage 
    (
        yellowTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    beginBackButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            core::position2di (0, buttonSize*3),
            core::dimension2di (buttonSize, buttonSize)
        ),
        beginWindow,
        EGID_BEGIN_BACK_BUTTON, // id
        L"Back" // button text
        //, L"Tooltip text"
    );
    beginBackButton->setSubElement (true);
    beginBackButton->setTabStop (false);
    beginBackButton->setPressedImage 
    (
        redTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonWidth, buttonHeight)
        )
    ); // }}}2

    // end beginWindow }}}

    settingsWindow = GUI->addWindow // {{{1
    (
        rect<s32>
        (
            position2di (0,0),
            core::dimension2di (buttonSize*numButtons, buttonSize*numButtons)
        ),
        false, // modal?
        L"Settings",
        titleWindow,
        EGID_SETTINGS_WINDOW
    );
    settingsWindow->setVisible (false);
    settingsWindow->setDraggable (false);
    settingsWindow->getCloseButton()->setVisible (false);
    settingsWindow->setSubElement (true);


    settingsPhysicsButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            position2di (buttonSize, buttonSize),
            core::dimension2di (buttonSize, buttonSize)
        ),
        settingsWindow,
        EGID_SETTINGS_PHYSICS_BUTTON, // id
        L"Physics" // button text
        //, L"Tooltip text"
    );
    settingsPhysicsButton->setSubElement (true);
    settingsPhysicsButton->setTabStop (false);
    settingsPhysicsButton->setPressedImage 
    (
        orangeTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); // }}}2

    settingsBackButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            core::position2di (0, buttonSize*3),
            core::dimension2di (buttonSize, buttonSize)
        ),
        settingsWindow,
        EGID_SETTINGS_BACK_BUTTON, // id
        L"Back" // button text
        //, L"Tooltip text"
    );
    settingsBackButton->setSubElement (true);
    settingsBackButton->setTabStop (false);
    settingsBackButton->setPressedImage 
    (
        redTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonWidth, buttonHeight)
        )
    ); // }}}2

    // end settingsWindow }}}1

    physicsWindow = GUI->addWindow // {{{1
    (
        rect<s32>
        (
            position2di (0,0),
            core::dimension2di (buttonSize*numButtons, buttonSize*numButtons)
        ),
        false, // modal?
        L"Physics",
        settingsWindow,
        EGID_PHYSICS_WINDOW
    );
    physicsWindow->setVisible (false);
    physicsWindow->setDraggable (false);
    physicsWindow->getCloseButton()->setVisible (false);
    physicsWindow->setSubElement (true);


    // xyz static text headers {{{2
    IGUIStaticText* x = GUI->addStaticText
    (
        L"x",
        rect<s32>
        (
            position2di (buttonSize +8, buttonSize/4),
            dimension2di (buttonSize/4, buttonSize/4)
        ),
        false, // border
        false, // wordwrap
        physicsWindow, // parent
        0
    );
    x->setSubElement (true); 
    IGUIStaticText* y = GUI->addStaticText
    (
        L"y",
        rect<s32>
        (
            position2di (s32(buttonSize*1.5 +8), s32(buttonSize/4)),
            dimension2di (buttonSize/4, buttonSize/4)
        ),
        false, // border
        false, // wordwrap
        physicsWindow, // parent
        0
    );
    y->setSubElement (true); 
    IGUIStaticText* z = GUI->addStaticText
    (
        L"z",
        rect<s32>
        (
            position2di (buttonSize*2 +8, buttonSize/4),
            dimension2di (buttonSize/4, buttonSize/4)
        ),
        false, // border
        false, // wordwrap
        physicsWindow, // parent
        0
    );
    z->setSubElement (true); 
    // end xyz headers }}}2

    worldGravText = GUI->addStaticText // {{{2
    (
        L"worldGrav",
        rect<s32>
        (
            position2di (8, buttonSize/2 +8),
            dimension2di (buttonSize, buttonSize/4)
        ),
        false, // border
        false, // wordwrap
        physicsWindow, // parent
        EGID_PHYSICS_WORLD_GRAV_TEXT // ID
    );
    worldGravText->setSubElement (true); // }}}2

    // worldGrav xyz spinBoxes {{{2
    worldGravXBox = GUI->addSpinBox 
    (
        L" ",
        rect<s32>
        (
            position2di (buttonSize, buttonSize/2),
            dimension2di (buttonSize/2, buttonSize/4)
        ),
        true,  // border
        physicsWindow, // parent
        EGID_PHYSICS_WORLD_GRAV_X  // ID
    );
    worldGravXBox->setSubElement (true);
    worldGravXBox->getEditBox()->setTextAlignment (EGUIA_CENTER, EGUIA_CENTER);
    worldGravXBox->setRange (-100,100);
    worldGravXBox->setDecimalPlaces (0);
    //worldGravXBox->setValue (worldGrav.x());

    worldGravYBox = GUI->addSpinBox
    (
        L" ",
        rect<s32>
        (
            position2di (s32(buttonSize*1.5), s32(buttonSize/2)),
            dimension2di (buttonSize/2, buttonSize/4)
        ),
        true,  // border
        physicsWindow, // parent
        EGID_PHYSICS_WORLD_GRAV_Y  // ID
    );
    worldGravYBox->setSubElement (true);
    worldGravYBox->getEditBox()->setTextAlignment (EGUIA_CENTER, EGUIA_CENTER);
    worldGravYBox->setRange (-100,100);
    worldGravYBox->setDecimalPlaces (0);
    //worldGravYBox->setValue (worldGrav.y());

    worldGravZBox = GUI->addSpinBox
    (
        L" ",
        rect<s32>
        (
            position2di (buttonSize*2, buttonSize/2),
            dimension2di (buttonSize/2, buttonSize/4)
        ),
        true,  // border
        physicsWindow, // parent
        EGID_PHYSICS_WORLD_GRAV_Z  // ID
    );
    worldGravZBox->setSubElement (true);
    worldGravZBox->getEditBox()->setTextAlignment (EGUIA_CENTER, EGUIA_CENTER);
    worldGravZBox->setRange (-100,100);
    worldGravZBox->setDecimalPlaces (0);
    //worldGravZBox->setValue (worldGrav.z());
    // }}}2

    IGUIStaticText* tCubeRestText = GUI->addStaticText // {{{2
    (
        L"tCubeRestitution",
        rect<s32>
        (
            position2di (8, buttonSize +8),
            dimension2di (buttonSize, buttonSize/4)
        ),
        false, // border
        false, // wordwrap
        physicsWindow, // parent
        0
    );
    tCubeRestText->setSubElement (true); // }}}2

    tCubeRestBox = GUI->addSpinBox // {{{2
    (
        L" ",
        rect<s32>
        (
            position2di (buttonSize, buttonSize),
            dimension2di (buttonSize/2, buttonSize/4)
        ),
        true,  // border
        physicsWindow, // parent
        EGID_PHYSICS_TCUBE_REST_BOX  // ID
    );
    tCubeRestBox->setSubElement (true);
    tCubeRestBox->getEditBox()->setTextAlignment (EGUIA_CENTER, EGUIA_CENTER);
    tCubeRestBox->setRange (0,1);
    tCubeRestBox->setDecimalPlaces (2);
    tCubeRestBox->setStepSize (.1f); // }}}2

    physicsBackButton = GUI->addButton // {{{2
    (
        core::rect<s32>
        (
            core::position2di (0, buttonSize*3),
            core::dimension2di (buttonSize, buttonSize)
        ),
        physicsWindow,
        EGID_PHYSICS_BACK_BUTTON, // id
        L"Back" // button text
        //, L"Tooltip text"
    );
    physicsBackButton->setSubElement (true);
    //physicsBackButton->setTabStop (true);
    physicsBackButton->setPressedImage 
    (
        redTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonWidth, buttonHeight)
        )
    ); // }}}2

    // end physicsWindow }}}

    saveWindow = GUI->addWindow // {{{
    (
        rect<s32>
        (
            position2di (screenCenter.X - buttonSize, screenCenter.Y - buttonSize),
            dimension2di (buttonSize*2, buttonSize*2)
        ),
        false, // modal
        L" Save", // title
        GUI->getRootGUIElement(), // parent
        EGID_SAVE_WINDOW // id
    );
    saveWindow->getCloseButton()->setVisible (false);
    saveWindow->setDraggable (false);
    saveWindow->setVisible (false);
    //saveWindow->setSubElement (false);


    IGUIStaticText* saveTxt = GUI->addStaticText 
    (
        L"Enter a name", // text
        rect<s32>
        (
            position2di (0, buttonSize/4),
            dimension2di (buttonSize*2, buttonSize*2)
        ), 
        false, // border
        true, // wordwrap
        saveWindow // parent
        //-1, // id
        //false  // fillBackground
    );
    saveTxt->setSubElement( true );
    saveTxt->setTextAlignment( EGUIA_CENTER, EGUIA_SCALE );

    saveEditBox = GUI->addEditBox
    ( 
        L"autoSave", 
        rect<s32>
        (
            position2di (buttonSize/2, buttonSize/2),
            dimension2di (buttonSize, buttonSize/4)
        ),
        true, // border
        saveWindow, // parent
        EGID_SAVE_EDITBOX // id
    );
    saveEditBox->setSubElement( true );
    saveEditBox->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );

    saveOkButton = GUI->addButton 
    (
        core::rect<s32>
        (
            core::position2di (buttonSize, buttonSize),
            core::dimension2di (buttonSize, buttonSize)
        ),
        saveWindow,
        EGID_SAVE_OK_BUTTON, // id
        L"OK" // button text
        //, L"Tooltip text"
    );
    saveOkButton->setSubElement (true);
    saveOkButton->setTabStop (false);
    saveOkButton->setPressedImage 
    (
        greenTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); 

    IGUIButton* saveCancel = GUI->addButton 
    (
        core::rect<s32>
        (
            core::position2di (0, buttonSize),
            core::dimension2di (buttonSize, buttonSize)
        ),
        saveWindow,
        EGID_SAVE_CANCEL_BUTTON, // id
        L"Cancel" // button text
        //, L"Tooltip text"
    );
    saveCancel->setSubElement (true);
    saveCancel->setTabStop (false);
    saveCancel->setPressedImage 
    (
        redTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    );
    // END SAVE WINDOW }}} 

	cursor->setVisible (false);
    cursor->setPosition (screenCenter);

    GUI->setFocus( titleWindow );

    GUIroot= GUI->getRootGUIElement();

} // end InitGUI

void kx::InitModal (const stringw& title, const stringw& text) // {{{1
{
    // creates modal window and focuses it

    IGUIWindow* modalWin = GUI->addWindow 
    (
        rect<s32>
        (
            position2di (screenCenter.X - buttonSize, screenCenter.Y - buttonSize),
            dimension2di (buttonSize*2, buttonSize*2)
        ),
        true, // modal
        title.c_str(), // title
        GUI->getRootGUIElement (), // parent
        EGID_MODAL_WIN // id
    );
    modalWin->getCloseButton()->setVisible (false);
    modalWin->setDraggable (false);

    IGUIStaticText* mtxt = GUI->addStaticText 
    (
        text.c_str(), // text
        rect<s32>
        (
            position2di (0, buttonSize/4),
            dimension2di (buttonSize*2, buttonSize*2)
        ), 
        false, // border
        true, // wordwrap
        modalWin // parent
        //-1, // id
        //false  // fillBackground
    );
    mtxt->setSubElement (true);

    IGUIButton* mOK = GUI->addButton 
    (
        core::rect<s32>
        (
            core::position2di (buttonSize, buttonSize),
            core::dimension2di (buttonSize, buttonSize)
        ),
        modalWin,
        EGID_LVL_PROGRESS_OK_BUTTON, // id
        L"OK" // button text
        //, L"Tooltip text"
    );
    mOK->setSubElement (true);
    mOK->setTabStop (false);
    mOK->setPressedImage 
    (
        greenTex, 
        core::rect<s32>
        (
            core::position2di (0,0),
            core::dimension2di (buttonSize, buttonSize)
        )
    ); 

    isGUIvisible=true;
    device->setEventReceiver (GUIEventReceiver);
    titleWindow->setVisible (false);
    GUI->setFocus (modalWin);

} // end MODAL WIN }}}1


void kx::InitTextures ()
{
    fileSys->changeWorkingDirectoryTo( kxBaseDir );

    playerTex = driver->getTexture ("./textures/player.png");
    yellowTex = driver->getTexture ("./textures/yellow.png");
    orangeTex = driver->getTexture ("./textures/orange.png");
    grayTex = driver->getTexture ("./textures/gray.png");
    darkGrayTex = driver->getTexture ("./textures/darkGray.png");
    blackTex = driver->getTexture ("./textures/black.png");
    whiteTex = driver->getTexture ("./textures/white.png");
    redTex = driver->getTexture ("./textures/red.png");
    greenTex = driver->getTexture ("./textures/green.png");
    darkGreenTex = driver->getTexture ("./textures/darkGreen.png");
    neonGreenTex = driver->getTexture ("./textures/neonGreen.png");
    blueTex = driver->getTexture ("./textures/blue.png");
    purpleTex = driver->getTexture ("./textures/purple.png");
    lightTex = driver->getTexture ("./textures/kxLight.png");
    soundToolTex = driver->getTexture ("./textures/kxSoundTool.png");

    playerMat = SMaterial();
    playerMat.GouraudShading= false;
    //playerMat.MaterialType= EMT_REFLECTION_2_LAYER;
    playerMat.setTexture( 0, playerTex ); 
    //playerMat.setTexture( 0, grayTex ); 
    //playerMat.EmissiveColor= SColor( 55,255,255,255 );


    whiteMat = video::SMaterial();
    whiteMat.setFlag (video::EMF_LIGHTING, false);
    //whiteMat.ZWriteEnable = false; // just draw on top
    
    blackMat = video::SMaterial();
    blackMat.setTexture (0, blackTex);
    blackMat.setFlag (video::EMF_LIGHTING, false);
}


void kx::DrawGUI () const
{
    GUI->drawAll();	

    // CURSOR
    core::position2d<s32> m = device->getCursorControl()->getPosition();
    driver->draw2DRectangle( SColor( 200, 100, 100, 100 ),
        core::rect<s32>( m.X, m.Y, m.X+32, m.Y+32 ));
    driver->draw2DRectangle( SColor( 200, 180, 180, 180 ),
        core::rect<s32>( m.X, m.Y, m.X+16, m.Y+16 ));

    driver->draw2DLine( m, m + vector2di( 32, 0 ), SColor( 255,255,255,255 ));
    driver->draw2DLine( m, m + vector2di( 0, 32 ), SColor( 255,255,255,255 ));
}

