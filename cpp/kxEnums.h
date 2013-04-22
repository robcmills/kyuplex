#ifndef __KX_ENUMS_H_INCLUDED__
#define __KX_ENUMS_H_INCLUDED__

// ENUMS {{{
enum EKX_NODE_TYPE
{
    KNT_ROOT_NODE = 101,
    KNT_BOX,
    KNT_RIGID_BOX,
    KNT_FRACTURE_CUBE,
    KNT_PLAYER_CUBE, 
    KNT_KILL_CUBE,
	KNT_CAMERA,
	KNT_CAM_PIVOT,
	//KNT_CAM_PIVOT_CHILD,
    KNT_TESTCUBE, 
    KNT_SHELLCUBE, 
    KNT_COLLECTABLE, 
	KNT_LEVEL,
    KNT_QUAD_PLANE,
    KNT_SPLINE_NODE,
    KNT_LIGHT,
    KNT_MESH
};

enum EKX_GAME_STATE
{
    EGS_GAME= 201,
    EGS_EDITOR,
    EGS_INIT
};

enum EKX_PLAYER_STATE
{
	EPS_WEAPON = 301,
    EPS_CUBE,
    EPS_DEAD
};

enum EKX_TOOLS
{
    KT_NONE = 401,
	KT_SOUND
};


enum EKX_PLAYER_ACTION 
{
    EPA_MOVE_FORWARD = 501,
    EPA_MOVE_BACKWARD,
    EPA_MOVE_LEFT,
    EPA_MOVE_RIGHT,
    EPA_MOVE_UP,
    EPA_MOVE_DOWN
};

enum EKX_CONTROL_TYPE 
{
    ECT_IMPULSE = 601,
    ECT_VELOCITY,
    ECT_NO_PHYSICS  
};

enum EKX_LEVEL_FORMAT
{
    ELF_KXL = 701,
    ELF_IRR
};

enum EKX_SPLINE_TYPE
{
    KST_MESH = 801,
    KST_SPLINE_OPEN, 
    KST_SPLINE_CLOSED,
    KST_CUBE,
    KST_NONE
};


#define BIT(x) ((signed short)1<<(x))

enum EKX_COLLISION_GROUPS 
{
    ECG_NULL = 0, 
    ECG_STATIC =        BIT(1),  // static level geometry
    ECG_DYNAMIC =       BIT(2),  
    ECG_PLAYER =   BIT(3), 
    ECG_RIGID_BOX =     BIT(4), 
    ECG_FRACTURE_CUBE = BIT(5), 
    ECG_COLLECTABLE =   BIT(6), // items, powerups, etc.
    ECG_GHOST =         BIT(7), // btGhostObject
    ECG_LIGHT =         BIT(8), 
    ECG_SOUND_TOOL =    BIT(9),
    ECG_SOUND_BOX =     BIT(10)
};


enum EKX_GUI_ID
{
	EGID_TITLE_WINDOW = 901,
        EGID_BEGIN_BUTTON,
        EGID_SETTINGS_BUTTON,
        EGID_EDIT_BUTTON,
        EGID_END_BUTTON,

    EGID_BEGIN_WINDOW,
        EGID_BEGIN_NEW_BUTTON,
        EGID_BEGIN_LOAD_BUTTON,
        EGID_BEGIN_SAVE_BUTTON,
        EGID_BEGIN_BACK_BUTTON,

	EGID_SETTINGS_WINDOW,
        EGID_SETTINGS_PHYSICS_BUTTON,
        EGID_SETTINGS_BACK_BUTTON,

	EGID_PHYSICS_WINDOW,
        EGID_PHYSICS_EDIT_GRAV,
        EGID_PHYSICS_BACK_BUTTON,
        EGID_PHYSICS_WORLD_GRAV_TEXT,
        EGID_PHYSICS_WORLD_GRAV_X,  
        EGID_PHYSICS_WORLD_GRAV_Y,
        EGID_PHYSICS_WORLD_GRAV_Z,
        EGID_PHYSICS_TCUBE_REST_BOX,

    EGID_MODAL_WIN,
    EGID_LVL_PROGRESS_OK_BUTTON, // TODO: s/MODAL_OK_BUTTON

    EGID_SAVE_WINDOW,
        EGID_SAVE_OK_BUTTON, 
        EGID_SAVE_CANCEL_BUTTON, 
        EGID_SAVE_EDITBOX,
    EGID_LOAD_DIALOG
};

// end ENUMS }}}

#endif
