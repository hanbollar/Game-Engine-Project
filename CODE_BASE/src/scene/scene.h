#pragma once

#include "globals.h"
#include "window_maintainer.h"
#include "scene_object.h"
#include "character.h"
#include "camera.h"
#include "shader_programs/shader_program.h"
#include "timer.h"
#include "audio_handler.h"

/*struct ScreenHandler{
    GLuint current_screen_texture_ = -1;
    GLuint SCREENTEXTURES[4] = { -1, -1, -1, -1 };
    std::shared_ptr<Drawable> screen_quad_ = nullptr;
};*/

class Scene : public WindowMaintainer {
private:
    shared_ptr<Timer> timer_;

	shared_ptr<Camera> camera_;
    shared_ptr<AudioHandler> audio_handler_;

    bool game_started_ = false;
    bool playing_ = false;

    //ScreenHandler screen_handler;

    SceneList on_scene_ = SceneList::START;

    // START
    bool start_selected_ = true;

    // CONTROLLER
    

    // MAINGAME
    vector<shared_ptr<SceneObject>> generic_scene_objects_;
    vector<shared_ptr<Character>> scene_characters_;
    shared_ptr<Character> user_character_ = nullptr;
    unique_ptr<SceneObject> screen_quad_ = nullptr;
    unique_ptr<SceneObject> sky_quad_ = nullptr;

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