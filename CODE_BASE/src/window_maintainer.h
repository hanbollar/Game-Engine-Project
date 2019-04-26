#pragma once

#include "globals.h"

// SHELL CLASS - class responsible for all items on the window's screen
class WindowMaintainer {
protected:
	// should only be used in classes that inherit this
	WindowMaintainer() {}
	~WindowMaintainer() {}

    bool window_still_running_ = true;

public:
	virtual void Update() = 0;
	virtual void KeyPressEvent(int key, int scancode, int action, int mods) = 0;
    virtual void ControllerEvents(const unsigned char *button_events, const float *axes) = 0;
	virtual void RunWithDefaultSetup() = 0;
    virtual const glm::mat4& ViewProjection() const = 0;
    virtual glm::mat4 ViewProjection() = 0;

    virtual void CloseWindow() { window_still_running_ = false; }
    virtual bool Running() { return window_still_running_; }
};
