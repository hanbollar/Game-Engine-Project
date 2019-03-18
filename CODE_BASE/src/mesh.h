#pragma once

#include "globals.h"
#include "file_loader.h"
#include "drawable.h"

class Mesh : public Drawable {
public:
	Mesh(bool default = false);
	~Mesh();
	
	void CreateDefaultMesh();
	void Load(const char* file_path, int file_type);

	//virtual void Create() override;
};