#pragma once

#include "globals.h"

#include "file_loader.h"

static const unsigned int NUM_G_GEN_TEXTURES = 4;

class DMesh {
public:
	DMesh();
	~DMesh();

	void CreateDefaultMesh();
	void Draw(glm::mat4& view_projection);

private:

	void GeometryPass(glm::mat4& view_projection);
	void LightingPass(glm::mat4& view_projection);

	//GLuint geometry_fragment_shader = 0;
	//GLuint lighting_fragment_shader = 0;
	
	GLuint fbo = 0; // frame buffer used for the deferred shading.

	GLuint gtex_general[4]; // This stores the fragment output geometry deferred textures in
							// order of position, diffuse, normal, texture coordinates.
							// Instead of glgen 4 different texture objects, it's technically
							// now one texture object that stores all 4 textures.
	GLuint gtex_depth = 0;

	GLuint model_view_projection_location = -1;
	glm::mat4 model_matrix = glm::mat4(1.f);
};