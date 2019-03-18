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
    GenericMesh,
    Billboard,
    ScreenspaceElement,

    NUMVISUALTYPES
} VisualType;