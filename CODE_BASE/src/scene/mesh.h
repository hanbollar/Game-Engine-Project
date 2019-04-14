#pragma once

#include "globals.h"
#include "file_loader.h"
#include "drawable.h"

class Mesh : public Drawable {
private:
    void CreateDefaultMesh();

public:
	Mesh();
    Mesh(const char* file_path, Filetype file_type);
	~Mesh();
	
    void Load(const char* file_path, Filetype mesh_file_type);

	//virtual void Create() override;
};