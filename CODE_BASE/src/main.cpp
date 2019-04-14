#include "globals.h"
#include "my_window.h"
#include "scene/scene.h"

int main(void) {
	// TODO: create an onScrene item maintainer.
	//when key press happens if it gets an action [within items of the onscreen maintainer]
	//returns true so scene does not do key press actions?
	Scene scene = Scene();
	MyWindow mainGui = MyWindow(&scene);

	return 0;
}