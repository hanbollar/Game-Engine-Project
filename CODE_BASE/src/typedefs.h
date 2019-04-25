#pragma once

typedef enum {
	VAO,
    TEX,
	// all below handles are generic buffers - update as needed.
	// if add another item that's not a buffer, add above this block.
	IBO,
	VBO,
	EBO,
	POS,
	COL,
	NOR,
	UVS,

	NUMHANDLES
} HandleType;

typedef enum {
    Y = 0,
    X = 1,
    A = 2,
    B = 3,
    LEFT_TAB = 4,
    RIGHT_TAB = 5,
    Z = 6,

    Start_Pause = 9,

    UP_ARROW = 12,
    RIGHT_ARROW = 13,
    DOWN_ARROW = 14,
    LEFT_ARROW = 15,

    NUMBUTTONS = 16
} JoystickButtons;

typedef enum {
    // character movement is the left one
    // camera rotation is the right one
    MOVE_X,
    MOVE_Y,
    CAMROT_X,
    CAMROT_Y,

    NUMAXES
} JoystickAxes;

typedef enum {
    OBJ,
    JSON,
    COLLADA,

    NUMFILETYPES
} Filetype;

typedef enum {
    START,
    CREDITS,
    CONTROLS,
    MAINGAME,

    NUMSCENES
} SceneList;

typedef enum {
    BELL = 0,
    STARTSCREEN = 1,

    NUMAUDIOS
} AudioChoices;

typedef enum {
    START_SELECTED = 0,
    CREDITS_SELECTED = 1,
    CONTROLS_SELECTED = 2,
    ON_CREDITS,
    ON_CONTROLS,
    
    NUM_TEXTURES
} StartTextures;

typedef enum {
    TIMER,
    GENERALSCREEN,

    NUM_TEXTIDS
} TextId;