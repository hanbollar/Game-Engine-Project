#include "scene.h"
#include "shader_programs/default_program.h"
#include "shader_programs/simple_program.h"

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
        generic_scene_objects_(vector<shared_ptr<SceneObject>>()),
        scene_characters_(vector<shared_ptr<Character>>()),
        audio_handler_(new AudioHandler()),
        timer_(shared_ptr<Timer>(new Timer()))
{}

Scene::Scene(Scene& s) {
    generic_scene_objects_ = s.generic_scene_objects_;
    scene_characters_ = s.scene_characters_;
	camera_ = s.camera();
    audio_handler_ = s.audio_handler_;
}

Scene::~Scene() {
	camera_.reset();
    generic_scene_objects_.clear();
    scene_characters_.clear();
}

const glm::mat4& Scene::ViewProjection() const {
    camera_->RecomputeAttributes();
    return camera_->GetViewProj();
}
glm::mat4 Scene::ViewProjection() {
    camera_->RecomputeAttributes();
    return camera_->GetViewProj();
}

void Scene::RunWithDefaultSetup() {

    // ShaderPrograms
	std::shared_ptr<ShaderProgram> rain_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
		"shader_files/threeD.vertexshader",
		"shader_files/rain.fragmentshader"));
    rain_prog->SetTimer(timer_);
    rain_prog->SetResolution(vec2(800, 800));
    std::shared_ptr<ShaderProgram> simple_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/simple.fragmentshader"));
    std::shared_ptr<ShaderProgram> sky_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/sky.fragmentshader"));
    std::shared_ptr<ShaderProgram> threeD_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/threeD.vertexshader",
        "shader_files/threeD.fragmentshader"));

    // SceneObjects
    user_character_ = shared_ptr<Character>(new Character(
        "resources/wahoo/wahoo.obj", Filetype::OBJ, rain_prog, "player", glm::vec3(0.f)));
    shared_ptr<SceneObject> random_mesh = shared_ptr<SceneObject>(new SceneObject(
        "resources/suzanne.obj", Filetype::OBJ, threeD_prog, "random", glm::vec3(0.f)));
    
    shared_ptr<SceneObject> rain_floor = shared_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, rain_prog, "rain floor", glm::vec3(0.f)));
    rain_floor->SetGlobalTransform(glm::rotate(glm::scale(glm::mat4(1.f), glm::vec3(3.f)), 90.f, glm::vec3(1, 0, 0)));

    screen_quad_ = unique_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, simple_program, "screenquad", glm::vec3(0.f)));
    sky_quad_ = unique_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, sky_program, "skyquad", glm::vec3(0.f)));

    ErrorHandler::PrintGLErrorLog();

    /*GLuint texture = -1;
    GLuint texture_buff = -1;
    ShaderProgram::LoadTextureFromFile("resources/wahoo/wahoo.bmp", texture, true);
    
    ShaderProgram::LoadTextureFromFile("resources/Start/play.jpg", start_selected_texture_, true);
    ShaderProgram::LoadTextureFromFile("resources/Start/controls.jpg", controller_selected_texture_, true);
    ShaderProgram::LoadTextureFromFile("resources/Start/controls.jpg", controller_texture_, true);
*/

    scene_characters_.push_back(user_character_);
	generic_scene_objects_.push_back(random_mesh);
    generic_scene_objects_.push_back(rain_floor);
}

void Scene::KeyPressEvent(int key, int scancode, int action, int mods) {
	// no longer using this but keeping here just in case for future.
}

void Scene::ControllerEvents(const unsigned char *button_events, const float *axes_events) {
    if (on_scene_ == SceneList::START) {
        if (button_events[JoystickButtons::A] == GLFW_PRESS) {
            std::cout << "SELECT" << std::endl;
            on_scene_ = start_selected_ ? MAINGAME : CONTROLLER;
            if (on_scene_ == SceneList::MAINGAME && !game_started_) {
                //RunWithDefaultSetup(); - this would have created the scene twice - change this to run game or sthg...
                audio_handler_->StartPlayingSound();
                audio_handler_->SetAudioSourcePos(glm::vec3(0.f));
                timer_->Play();
                playing_ = true;
            }

            audio_handler_->PlaySingleSound(AudioChoices::BELL);
        }

        if (abs(axes_events[JoystickAxes::MOVE_Y]) == 1) {
            std::cout << "SWITCH OPTION" << std::endl;
            start_selected_ = !start_selected_;

            /*if (screen_handler_->current_texture_ == screen_handler_->start_screen_start_selected) {
                screen_handler_->current_texture_ = screen_handler_->start_screen_controls_selected;
            } else {
                screen_handler_->current_texture_ = screen_handler_->start_screen_start_selected;
            }*/
        }
        
        return;
    } else if (on_scene_ == SceneList::CONTROLLER) {
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            std::cout << "EXIT CONTROLS" << std::endl;
            if (game_started_) {
                on_scene_ = SceneList::MAINGAME;
                timer_->Play();
                playing_ = true;
            } else {
                on_scene_ = START;
               // screen_handler_->current_texture_ = screen_handler_->start_screen_start_selected;
                start_selected_ = true;
            }
        }

        return;
    } else if (on_scene_ == SceneList::MAINGAME) {
        if (button_events[JoystickButtons::A] == GLFW_PRESS) {
            user_character_->Jump(camera_);
        }
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            user_character_->Crouch(camera_);
            audio_handler_->PlaySingleSound(AudioChoices::BELL);
        }

        if (button_events[JoystickButtons::RIGHT_TAB] == GLFW_PRESS) {
            camera_->TranslateAlongLook(0.5f);
        }
        if (button_events[JoystickButtons::LEFT_TAB] == GLFW_PRESS) {
            camera_->TranslateAlongLook(-0.5f);
        }

        if (button_events[JoystickButtons::Start_Pause] == GLFW_PRESS) {
            on_scene_ = CONTROLLER;
            playing_ = false;
            timer_->Pause();
        }

        if (axes_events[JoystickAxes::MOVE_X] == -1) {
            user_character_->MoveInDirection(camera_, glm::vec3(1, 0, 0));
        } else if (axes_events[JoystickAxes::MOVE_X] == 1) {
            user_character_->MoveInDirection(camera_, glm::vec3(-1, 0, 0));
        }

        if (axes_events[JoystickAxes::MOVE_Y] == 1) {
            user_character_->MoveInDirection(camera_, glm::vec3(0, 0, -1));
        } else if (axes_events[JoystickAxes::MOVE_Y] == -1) {
            user_character_->MoveInDirection(camera_, glm::vec3(0, 0, 1));
        }

        if (axes_events[JoystickAxes::CAMROT_Y] == -1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateVerticallyAboutPoint(-0.01f);
        } else if (axes_events[JoystickAxes::CAMROT_Y] == 1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateVerticallyAboutPoint(0.01f);
        }

        if (axes_events[JoystickAxes::CAMROT_X] == -1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateHorizontallyAboutPoint(-0.01f);
        } else if (axes_events[JoystickAxes::CAMROT_X] == 1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateHorizontallyAboutPoint(0.01f);
        }

        // update viewing elements since camera and user were modified
        camera_->RecomputeAttributes();

        return;
    } else {
        ErrorHandler::ThrowError("on invalid scene");
    }
}

void Scene::Update() {
    if (playing_) {
        audio_handler_->Update(user_character_->GetGlobalPosition());
        timer_->Tick();
    }

    switch (on_scene_)
    {
    case SceneList::MAINGAME:
        glDisable(GL_DEPTH_TEST);
        sky_quad_->Draw(this->ViewProjection());
        glEnable(GL_DEPTH_TEST);
        for (auto& so : generic_scene_objects_) {
            so->Draw(this->ViewProjection());
        }
        for (auto& c : scene_characters_) {
            c->Draw(this->ViewProjection());
        }

        break;
    case SceneList::START:
        /*if (screen_quad_)
            default_program_->Draw(screen_quad_, glm::mat4(1.f), this->ViewProjection());*/
        screen_quad_->Draw(this->ViewProjection());
        break;
    case SceneList::CONTROLLER:
        /*if (screen_quad_)
            default_program_->Draw(screen_quad_, glm::mat4(1.f), this->ViewProjection());*/
        screen_quad_->Draw(this->ViewProjection());
        break;
    default: 
        ErrorHandler::ThrowError("On invalid Scene");
    }
}