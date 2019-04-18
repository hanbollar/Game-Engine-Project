#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
using namespace std;

// Include GLEW
#include <GL/glew.h>
#include <GL/gl.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

static const unsigned int SCR_WIDTH = 1024;
static const unsigned int SCR_HEIGHT = 768;

#define M_PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862

// below namespace was converted from the 2017 glwidget277.h Qt based class from CIS 561
namespace ErrorHandler {
	string GLErrorToString(GLenum err);
	void ThrowError(string message);
	void ShowError(string message);
	void ThrowError(GLenum err);
	void ShowError(GLenum err);
	void OpenGLCheck();
	void GlslCheck();
	void PrintGLErrorLog(bool throwing = true);
	void PrintLinkInfoLog(int prog);
	void PrintShaderInfoLog(int shader);
};