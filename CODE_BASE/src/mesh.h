#pragma once

#include "globals.h"
#include "file_loader.h"
#include "drawable.h"

class Mesh : public Drawable {
public:
	Mesh();
    Mesh(const char* file_path, int file_type);
	~Mesh();
	
	void CreateDefaultMesh();
	void Load(const char* file_path, int file_type);

	//virtual void Create() override;
};