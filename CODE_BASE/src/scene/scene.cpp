#include "scene.h"
#include "shader_programs/default_program.h"
#include "shader_programs/simple_program.h"

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
        generic_scene_objects_(vector<shared_ptr<SceneObject>>()),
        scene_characters_(vector<shared_ptr<Character>>()),
        audio_handler_(new AudioHandler())
{ }

Scene::Scene(Scene& s) {
    generic_scene_objects_ = s.generic_scene_objects_;
    scene_characters_ = s.scene_characters_;
	*default_program_ = *(s.default_program_);
	camera_ = s.camera();
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
	default_program_ = std::shared_ptr<DefaultProgram>(new DefaultProgram(
		"shader_files/threeD.vertexshader",
		"shader_files/threeD.fragmentshader"));
    /*simple_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/simple.fragmentshader"));
    */
    user_character_ = shared_ptr<Character>(new Character(
        "resources/wahoo/wahoo.obj", Filetype::OBJ, default_program_, "player", glm::vec3(0.f)));

    shared_ptr<SceneObject> random_mesh = shared_ptr<SceneObject>(new SceneObject(
        "resources/buffalo/Bufalo_OBJ.obj", Filetype::OBJ, default_program_, "random", glm::vec3(0.f)));

    /*GLuint texture = -1;
    GLuint texture_buff = -1;
    ShaderProgram::LoadTextureFromFile("resources/wahoo/wahoo.bmp", texture, true);
    
    ShaderProgram::LoadTextureFromFile("resources/Start/play.jpg", start_selected_texture_, true);
    ShaderProgram::LoadTextureFromFile("resources/Start/controls.jpg", controller_selected_texture_, true);
    ShaderProgram::LoadTextureFromFile("resources/Start/controls.jpg", controller_texture_, true);

    ErrorHandler::PrintGLErrorLog();*/

    scene_characters_.push_back(user_character_);
	generic_scene_objects_.push_back(random_mesh);

    //ErrorHandler::PrintGLErrorLog();*/
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
            }

            audio_handler_->PlaySingleSound("resources/AUDIO/irrklang/bell.wav");
        }

        if (abs(axes_events[JoystickAxes::MOVE_Y]) == 1) {
            std::cout << "SWITCH OPTION" << std::endl;
            start_selected_ = !start_selected_;

            if (screen_quad_) {
                if (start_selected_) {
                    screen_quad_->SetHandleLocation(HandleType::TEX, start_selected_texture_);
                } else {
                    screen_quad_->SetHandleLocation(HandleType::TEX, controller_selected_texture_);
                }
            }
        }
        
        return;
    } else if (on_scene_ == SceneList::CONTROLLER) {
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            std::cout << "EXIT CONTROLS" << std::endl;
            if (game_started_) {
                on_scene_ = SceneList::MAINGAME;
                playing_ = true;
            } else {
                on_scene_ = START;
                if (screen_quad_) {
                    screen_quad_->SetHandleLocation(HandleType::TEX, start_selected_texture_);
                }
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
            audio_handler_->PlaySingleSound("resources/AUDIO/irrklang/bell.wav");
        }

        if (button_events[JoystickButtons::RIGHT_TAB] == GLFW_PRESS) {
            camera_->TranslateAlongLook(0.5f);
        }
        if (button_events[JoystickButtons::LEFT_TAB] == GLFW_PRESS) {
            camera_->TranslateAlongLook(-0.5f);
        }

        if (button_events[JoystickButtons::Start_Pause] == GLFW_PRESS) {
            on_scene_ = CONTROLLER;
        }

        if (axes_events[JoystickAxes::MOVE_X] == -1) {
            user_character_->MoveInDirection(camera_, glm::vec3(-1, 0, 0));
        } else if (axes_events[JoystickAxes::MOVE_X] == 1) {
            user_character_->MoveInDirection(camera_, glm::vec3(1, 0, 0));
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
        // TODO:: update timer?
    }

    switch (on_scene_)
    {
    case SceneList::MAINGAME:
        for (auto& so : generic_scene_objects_) {
            so->Draw(this->ViewProjection());
        }
        for (auto& c : scene_characters_) {
            c->Draw(this->ViewProjection());
        }

        break;
    case SceneList::START:
        if (screen_quad_)
            default_program_->Draw(screen_quad_, glm::mat4(1.f), this->ViewProjection());

        break;
    case SceneList::CONTROLLER:
        if (screen_quad_)
            default_program_->Draw(screen_quad_, glm::mat4(1.f), this->ViewProjection());

        break;
    default: 
        ErrorHandler::ThrowError("On invalid Scene");
    }
}