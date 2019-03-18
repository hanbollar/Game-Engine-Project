#pragma once

#include "window_maintainer.h"
#include "mesh.h"
#include "camera.h"
#include "shader_program.h"
#include "default_program.h"
#include "drawable.h"

class Scene : public WindowMaintainer {
private:
	vector<shared_ptr<Drawable>> scene_objects_;
	shared_ptr<Camera> camera_;

public:
	Scene();
	Scene(Scene& s);
	~Scene();

	ShaderProgram* default_program = nullptr;

	virtual void Render() override;
	virtual void KeyPressEvent(int key, int scancode, int action, int mods) override;
	virtual void RunWithDefaultSetup() override;

    virtual const glm::mat4& ViewProjection() const override;
    virtual glm::mat4 ViewProjection() override;

	const vector<shared_ptr<Drawable>> scene_objects() const { return scene_objects_; }
	vector<shared_ptr<Drawable>> scene_objects() { return scene_objects_; }
	const shared_ptr<Camera> camera() const { return camera_; }
	shared_ptr<Camera> camera() { return camera_; }
};