#pragma once

#include "globals.h"
#include "window_maintainer.h"
#include "scene_object.h"
#include "character.h"
#include "camera.h"
#include "shader_programs/shader_program.h"
#include "timer.h"
#include "audio_handler.h"
#include "text_handler.h"

class Scene : public WindowMaintainer {
private:
    shared_ptr<Timer> timer_;

	shared_ptr<Camera> camera_;
    shared_ptr<AudioHandler> audio_handler_;
    shared_ptr<TextHandler> text_handler_;

    //shared_ptr<DefaultProgram> rain_prog_; // TODO: need a better way to interface with this...

    bool game_started_ = false;
    bool playing_ = false;

    //ScreenHandler screen_handler;

    SceneList on_scene_ = SceneList::START;

    // LOADING
    GLuint loadingTextures_[3];

    // START SCREEN
    int on_selected_ = 0;
    GLuint selected_[5]; // start_selected. controls_selected. credits_selected. on_controls. on_credits.
    
    // MAINGAME
    vector<shared_ptr<SceneObject>> generic_scene_objects_;
    vector<shared_ptr<Character>> scene_characters_;

    shared_ptr<Character> user_character_ = nullptr;
    unique_ptr<SceneObject> screen_quad_ = nullptr;
    unique_ptr<SceneObject> sky_quad_ = nullptr;

    shared_ptr<Drawable> blank_quad_ = nullptr;

public:
	Scene();
    Scene(Scene& s);
	~Scene();

	virtual void Update();
	virtual void KeyPressEvent(int key, int scancode, int action, int mods);
    virtual void ControllerEvents(const unsigned char *button_events, const float *axes);
	virtual void RunWithDefaultSetup();

    void RunLoadingScreen();
    void StartGame();
    
    void RandomlyAddToScene(
        std::shared_ptr<SceneObject> scene_obj, const unsigned int& num, 
        const glm::fvec3& min_scale, const glm::fvec3&max_scale,
        const glm::ivec3& rand_dim, const glm::vec3& center);

    virtual const glm::mat4& ViewProjection() const;
    virtual glm::mat4 ViewProjection();

	const vector<shared_ptr<SceneObject>> scene_objects() const { return generic_scene_objects_; }
	vector<shared_ptr<SceneObject>> scene_objects() { return generic_scene_objects_; }
	const shared_ptr<Camera> camera() const { return camera_; }
	shared_ptr<Camera> camera() { return camera_; }
};