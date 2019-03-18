#include "my_window.h"

MyWindow::MyWindow(WindowMaintainer* m)
	: MyWindow(1024, 768, m) {}

MyWindow::MyWindow(int w, int h, WindowMaintainer* m)
	: width(w), height(h), maintainer(m) {

	initWindow();

	// set up connection between window and its maintainer
	glfwSetWindowUserPointer(the_window, m);

	run();
}

MyWindow::~MyWindow() {
	glfwDestroyWindow(the_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

/********************************/
/****** callback functions ******/
/********************************/

void MyWindow::key_event_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER || key == GLFW_KEY_END) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	WindowMaintainer* m = reinterpret_cast<WindowMaintainer *>(glfwGetWindowUserPointer(window));
	m->KeyPressEvent(key, scancode, action, mods);
}

void MyWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// readjusting the camera's dimensions and fov
	//Scene* scene = reinterpret_cast<Scene *>(glfwGetWindowUserPointer(window));
	//width = (int)(height * scene->camera.aspect);
	//scene->camera.resize(width, height);
	//cout << "camera resized" << endl;*/

	// used when the window is first sized or when the window is resizing
	// makes sure framebuffer and window match sizes so swapping works
	//glfwGetFramebufferSize(window, &width, &height);

	// actually adjusts the size of the glviewport to match the new size
	glViewport(0, 0, width, height);
}

void MyWindow::error_callback(int error, const char* description) {
	ErrorHandler::ShowError(error + ": " + string(description));
}

/******************************/
/****** window functions ******/
/******************************/

void MyWindow::initWindow() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	// set up error handler
	glfwSetErrorCallback(error_callback);
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Open a window and create its OpenGL context
	the_window = glfwCreateWindow(width, height, "Game Engine", NULL, NULL);
	if (!the_window) {
		fprintf(stderr, "MyWindow::initWindow - ERROR - `the window` failed to load.\n");
		getchar();
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	// allows for all context related calls - must be created before glewinit
	glfwMakeContextCurrent(the_window);
	ErrorHandler::PrintGLErrorLog();

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	ErrorHandler::PrintGLErrorLog(0);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(the_window, width / 2, height / 2);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(the_window, GLFW_STICKY_KEYS, GL_TRUE);

	// "background" color
	glClearColor(0.1f, 0.1f, 1.f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// time between buffer and screen swaps
	glfwSwapInterval(1);

	// connection between mainWindow and event functions
	glfwSetKeyCallback(the_window, key_event_callback);
	glfwSetFramebufferSizeCallback(the_window, framebuffer_size_callback);

	// any errors in GL initialization?
	ErrorHandler::OpenGLCheck();
	ErrorHandler::GlslCheck();
	ErrorHandler::PrintGLErrorLog();
}

void MyWindow::run() {
	maintainer->RunWithDefaultSetup();

	while (!glfwWindowShouldClose(the_window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		maintainer->Render();

		glfwSwapBuffers(the_window);
		glfwPollEvents();
	}
	glfwDestroyWindow(the_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}