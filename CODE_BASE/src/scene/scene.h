#pragma once

#include "globals.h"
#include "window_maintainer.h"
#include "scene_object.h"
#include "character.h"
#include "camera.h"
#include "shader_programs/shader_program.h"
#include "audio_handler.h"

class Scene : public WindowMaintainer {
private:
    GLuint program_test_;
    GLuint vao_;
    GLuint vbo_;
    int num_pos_;

	shared_ptr<Camera> camera_;
    shared_ptr<AudioHandler> audio_handler_;

    shared_ptr<ShaderProgram> default_program_;
    shared_ptr<ShaderProgram> simple_program_;

    bool game_started_ = false;
    bool playing_ = false;

    SceneList on_scene_ = SceneList::START;

    // START
    shared_ptr<Drawable> screen_quad_ = nullptr;
    GLuint start_selected_texture_ = -1;
    GLuint controller_selected_texture_ = -1;
    bool start_selected_ = true;

    // CONTROLLER
    GLuint controller_texture_ = -1;

    // MAINGAME
    vector<shared_ptr<SceneObject>> generic_scene_objects_;
    vector<shared_ptr<Character>> scene_characters_;
    shared_ptr<Character> user_character_ = nullptr;

public:
	Scene();
	Scene(Scene& s);
	~Scene();

	virtual void Update();
	virtual void KeyPressEvent(int key, int scancode, int action, int mods);
    virtual void ControllerEvents(const unsigned char *button_events, const float *axes);
	virtual void RunWithDefaultSetup();

    virtual const glm::mat4& ViewProjection() const;
    virtual glm::mat4 ViewProjection();

	const vector<shared_ptr<SceneObject>> scene_objects() const { return generic_scene_objects_; }
	vector<shared_ptr<SceneObject>> scene_objects() { return generic_scene_objects_; }
	const shared_ptr<Camera> camera() const { return camera_; }
	shared_ptr<Camera> camera() { return camera_; }
};