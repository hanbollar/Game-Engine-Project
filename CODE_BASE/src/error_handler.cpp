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
	cout << ErrorHandler::GLErrorToString(err) << endl;
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
	cerr << "IMPLEMENT ME::::" << endl;
	/*_GLFWwindow *ctx = glfwGetCurrentContext();
	QSurfaceFormat form = format();
	QSurfaceFormat ctxform = ctx->format();
	QSurfaceFormat::OpenGLContextProfile prof = ctxform.profile();

	const char *profile =
	prof == QSurfaceFormat::CoreProfile ? "Core" :
	prof == QSurfaceFormat::CompatibilityProfile ? "Compatibility" :
	"None";
	int ctxmajor = ctxform.majorVersion();
	int ctxminor = ctxform.minorVersion();
	bool valid = ctx->isValid();
	int formmajor = form.majorVersion();
	int formminor = form.minorVersion();
	const char *vendor = glGS(GL_VENDOR);
	const char *renderer = glGS(GL_RENDERER);
	const char *version = glGS(GL_VERSION);
	const char* s_glsl = glGS(GL_SHADING_LANGUAGE_VERSION);

	printf("Widget version: %d.%d\n", ctxmajor, ctxminor);
	printf("Context valid: %s\n", valid ? "yes" : "NO");
	printf("Format version: %d.%d\n", formmajor, formminor);
	printf("Profile: %s\n", profile);
	printf("  Vendor:   %s\n", vendor);
	printf("  Renderer: %s\n", renderer);
	printf("  Version:  %s\n", version);
	printf("  GLSL:     %s\n", s_glsl);

	QString glsl = s_glsl;
	if (ctxmajor < 3 || glsl.startsWith("1.10") || glsl.startsWith("1.20")) {
	printf("ERROR: "
	"Unable to get an OpenGL 3.x context with GLSL 1.30 or newer. "
	"If your hardware should support it, update your drivers. "
	"If you have switchable graphics, make sure that you are using the discrete GPU.\n");
	QApplication::exit();
	}
	else if ((ctxmajor == 3 && ctxminor < 2) || glsl.startsWith("1.30") || glsl.startsWith("1.40")) {
	printf("WARNING: "
	"Enable to get an OpenGL 3.2 context with GLSL 1.50. "
	"If your hardware should support it, update your drivers. "
	"If you have switchable graphics, make sure that you are using the discrete GPU. "
	"If you cannot get 3.2 support, it is possible to port this project....");

	// Note: doing this requires at least the following actions:
	// * Change the header and base class in glwidget277.h to 3.0/3.1 instead of 3.2 Core.
	// * Change the shaders to require GLSL 1.30 or 1.40.
	}*/
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