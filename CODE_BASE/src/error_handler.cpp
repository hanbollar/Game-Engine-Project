#include "globals.h"

string ErrorHandler::GLErrorToString(GLenum err) {
	switch (err) {
	case GL_NO_ERROR:                      return "GL_NO_ERROR";
	case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
	case GL_INVALID_INDEX:				   return "GL_INVALID_INDEX";
	case 0x8031: /* not core */            return "GL_TABLE_TOO_LARGE_EXT";
	case 0x8065: /* not core */            return "GL_TEXTURE_TOO_LARGE_EXT";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	default:
		assert(!"Unhandled GL error code");
		return NULL;
	}
}

void ErrorHandler::ThrowError(string message) {
	cout << message << endl;
	throw;
}

void ErrorHandler::ShowError(string message) {
	cout << message << endl;
}

void ErrorHandler::ThrowError(GLenum err) {
    cout << glewGetErrorString(err) << endl;
	//cout << ErrorHandler::GLErrorToString(err) << endl;
	throw;
}

void ErrorHandler::ShowError(GLenum err) {
	cout << ErrorHandler::GLErrorToString(err) << endl;
}

void ErrorHandler::OpenGLCheck() {
	cout << "OpenGL Vendor: " << (char*)glGetString(GL_VENDOR) << endl;
	cout << "OpenGL Renderer: " << (char*)glGetString(GL_RENDERER) << endl;
	cout << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << endl;
	cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
}

void ErrorHandler::GlslCheck() {
    printf("Compiled against GLFW %i.%i.%i\n",
        GLFW_VERSION_MAJOR,
        GLFW_VERSION_MINOR,
        GLFW_VERSION_REVISION);
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
}

void ErrorHandler::PrintGLErrorLog(bool throwing) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		ShowError(error);
		// Throwing here allows us to use the debugger to track down the error.
#ifndef __APPLE__
		// Don't do this on OS X.
		// http://lists.apple.com/archives/mac-opengl/2012/Jul/msg00038.html
		if (throwing) 
			throw;
#endif
	}
}

void ErrorHandler::PrintLinkInfoLog(int prog) {
	GLint linked;
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	if (linked == GL_TRUE) {
		return;
	}
	cout << "GLSL LINK ERROR" << endl;

	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0) {
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
		cout << "InfoLog:" << endl << infoLog << endl;
		delete[] infoLog;
	}
	throw;
}

void ErrorHandler::PrintShaderInfoLog(int shader) {
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_TRUE) {
		return;
	}
	cerr << "GLSL COMPILE ERROR" << endl;

	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0) {
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
		cout << "InfoLog:" << endl << infoLog << endl;
		delete[] infoLog;
	}
	throw;
}