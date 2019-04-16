#pragma once

#include "globals.h"
#include "window_maintainer.h"
#include "scene/camera.h"

typedef enum {
	GUI = 0,
	ERR,
	ETC // use these cases later to handle diff key press events etc
} WindowType; // TODO: do i really need this different enums?? - not using these anywhere yet

class MyWindow {
public:
	MyWindow(WindowMaintainer* m);
	MyWindow(int width, int height, WindowMaintainer* m);
	~MyWindow();

	int width = SCR_WIDTH;
	int height = SCR_HEIGHT;

	GLFWwindow* the_window;
	WindowMaintainer* maintainer;

	static void key_event_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void error_callback(int error, const char* description);

	void initWindow();
	void run();
};