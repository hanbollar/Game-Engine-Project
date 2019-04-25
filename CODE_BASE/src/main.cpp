#include "globals.h"
#include "my_window.h"
#include "scene/scene.h"

int main(void) {
	Scene scene = Scene();
	MyWindow mainGui = MyWindow(&scene);

	return 0;
}