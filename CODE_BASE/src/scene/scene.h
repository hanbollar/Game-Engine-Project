#pragma once

#include "window_maintainer.h"
#include "mesh.h"
#include "camera.h"
#include "shader_programs/shader_program.h"
#include "shader_programs/default_program.h"
#include "shader_programs/simple_program.h"
#include "drawable.h"
#include "character.h"
#include "audio_handler.h"

class Scene : public WindowMaintainer {
private:
	shared_ptr<Camera> camera_;
    shared_ptr<AudioHandler> audio_handler_;

    SceneList on_scene_ = SceneList::START;

    bool playing_ = false;
    bool game_started_ = true;

    // START
    shared_ptr<Drawable> screen_quad_ = nullptr;
    GLuint start_selected_texture_ = -1;
    GLuint controller_selected_texture_ = -1;
    bool start_selected_ = true;

    // CONTROLLER
    GLuint controller_texture_ = -1;

    // MAINGAME
    vector<shared_ptr<Drawable>> generic_scene_objects_;
    vector<shared_ptr<Character>> scene_characters_;

public:
	Scene();
	Scene(Scene& s);
	~Scene();

	ShaderProgram* default_program = nullptr;
    ShaderProgram* simple_program = nullptr;
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