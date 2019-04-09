#pragma once

#include "window_maintainer.h"
#include "mesh.h"
#include "camera.h"
#include "shader_program.h"
#include "default_program.h"
#include "drawable.h"
#include "character.h"

class Scene : public WindowMaintainer {
private:
	vector<shared_ptr<Drawable>> generic_scene_objects_;
    vector<shared_ptr<Character>> scene_characters_;
	shared_ptr<Camera> camera_;

public:
	Scene();
	Scene(Scene& s);
	~Scene();

	ShaderProgram* default_program = nullptr;
    shared_ptr<Character> user_character = nullptr;

	virtual void Update();
	virtual void KeyPressEvent(int key, int scancode, int action, int mods);
    virtual void ControllerEvents(const unsigned char *button_events, const float *axes);
	virtual void RunWithDefaultSetup();

    virtual const glm::mat4& ViewProjection() const;
    virtual glm::mat4 ViewProjection();

	const vector<shared_ptr<Drawable>> scene_objects() const { return generic_scene_objects_; }
	vector<shared_ptr<Drawable>> scene_objects() { return generic_scene_objects_; }
	const shared_ptr<Camera> camera() const { return camera_; }
	shared_ptr<Camera> camera() { return camera_; }
};