#include "scene.h"

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
        generic_scene_objects_(vector<shared_ptr<Drawable>>()),
        scene_characters_(vector<shared_ptr<Character>>()),
        audio_handler_(new AudioHandler()){
}

Scene::Scene(Scene& s) {
    generic_scene_objects_ = s.generic_scene_objects_;
    scene_characters_ = s.scene_characters_;
	*default_program = *(s.default_program);
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
    return camera_->GetViewProj();
}

void Scene::RunWithDefaultSetup() {
	default_program = new DefaultProgram(
		"shader_files/threeD.vertexshader",
		"shader_files/threeD.fragmentshader");

    simple_program = new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/simple.fragmentshader");

    shared_ptr<Drawable> main_mesh = static_cast<std::shared_ptr<Drawable>>(new Mesh("resources/wahoo/wahoo.obj", Filetype::OBJ));
    user_character = shared_ptr<Character>(new Character(main_mesh, "player", glm::vec3(0.f)));

    shared_ptr<Drawable> random_mesh = static_cast<std::shared_ptr<Drawable>>(new Mesh("resources/wahoo/wahoo.obj", Filetype::OBJ));

    GLuint texture = -1;
    ShaderProgram::LoadTextureFromFile("resources/wahoo/wahoo.bmp", texture, true);

    default_program->CreateDrawable(main_mesh, start_selected_texture_);
    default_program->CreateDrawable(random_mesh, texture);

    //screen_quad_ = random_mesh;
    screen_quad_ = shared_ptr<Drawable>(new Drawable());
    screen_quad_->MakeScreenQuad();
    
    ShaderProgram::LoadTextureFromFile("resources/Start/play.jpg", start_selected_texture_, true);
    ShaderProgram::LoadTextureFromFile("resources/Start/controls.jpg", controller_selected_texture_, true);
    ShaderProgram::LoadTextureFromFile("resources/Start/controls.jpg", controller_texture_, true);

    default_program->CreateDrawable(screen_quad_, start_selected_texture_);

	generic_scene_objects_.push_back(random_mesh);
    scene_characters_.push_back(user_character);

    audio_handler_->StartPlayingSound();

    // setup
    /*float posOnCircle = 0;
    const float radius = 5;
    posOnCircle += 0.04f;
    vec3df pos3d(radius * cosf(posOnCircle), 0,
        radius * sinf(posOnCircle * 0.5f));
    // update - 
    vec3df pos(fmodf((float)rand(), radius * 2) - radius, 0, 0);*/
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
                RunWithDefaultSetup();
            }
        }

        if (abs(axes_events[JoystickAxes::MOVE_Y]) == 1) {
            std::cout << "SWITCH OPTION" << std::endl;
            start_selected_ != start_selected_;
            if (start_selected_) {
                screen_quad_->SetHandleLocation(HandleType::TEX, start_selected_texture_);
            } else {
                screen_quad_->SetHandleLocation(HandleType::TEX, controller_selected_texture_);
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
                screen_quad_->SetHandleLocation(HandleType::TEX, start_selected_texture_);
                start_selected_ = true;
            }
        }

        return;
    } else if (on_scene_ == SceneList::MAINGAME) {
        // not using switch case so can have multiple at one time
        if (button_events[JoystickButtons::A] == GLFW_PRESS) {
            std::cout << "JUMP" << std::endl;
            user_character->Jump();
        }
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            std::cout << "CROUCH" << std::endl;
            user_character->Crouch();
        }

        if (button_events[JoystickButtons::RIGHT_TAB] == GLFW_PRESS) {
            std::cout << "ZOOM IN" << std::endl;
            camera_->TranslateAlongLook(0.5f);
            camera_->RecomputeAttributes();
        }
        if (button_events[JoystickButtons::LEFT_TAB] == GLFW_PRESS) {
            std::cout << "ZOOM OUT" << std::endl;
            camera_->TranslateAlongLook(-0.5f);
            camera_->RecomputeAttributes();
        }

        if (button_events[JoystickButtons::Start_Pause] == GLFW_PRESS) {
            std::cout << "START - SEE CONTROLS" << std::endl;
            on_scene_ = CONTROLLER;
        }

        if (axes_events[JoystickAxes::MOVE_X] == -1) {
            std::cout << "MOVE LEFT" << std::endl;
            user_character->MoveLeft();
            camera_->RecomputeAttributes();
        }
        else if (axes_events[JoystickAxes::MOVE_X] == 1) {
            std::cout << "MOVE RIGHT" << std::endl;
            user_character->MoveRight();
            camera_->RecomputeAttributes();
        }

        if (axes_events[JoystickAxes::MOVE_Y] == 1) {
            std::cout << "MOVE FORWARD" << std::endl;
            user_character->MoveForward();
            camera_->RecomputeAttributes();
        }
        else if (axes_events[JoystickAxes::MOVE_Y] == -1) {
            std::cout << "MOVE BACKWARD" << std::endl;
            user_character->MoveBackward();
            camera_->RecomputeAttributes();
        }

        if (axes_events[JoystickAxes::CAMROT_Y] == -1) {
            std::cout << "ROT VIEW UP" << std::endl;
            camera_->ref = user_character->GetGlobalPosition();
            camera_->RotateVerticallyAboutPoint(-0.01f);
            camera_->RecomputeAttributes();
        }
        else if (axes_events[JoystickAxes::CAMROT_Y] == 1) {
            std::cout << "ROT VIEW DOWN" << std::endl;
            camera_->ref = user_character->GetGlobalPosition();
            camera_->RotateVerticallyAboutPoint(0.01f);
            camera_->RecomputeAttributes();
        }

        if (axes_events[JoystickAxes::CAMROT_X] == -1) {
            std::cout << "ROT VIEW LEFT" << std::endl;
            camera_->ref = user_character->GetGlobalPosition();
            camera_->RotateHorizontallyAboutPoint(-0.01f);
            camera_->RecomputeAttributes();
        }
        else if (axes_events[JoystickAxes::CAMROT_X] == 1) {
            std::cout << "ROT VIEW RIGHT" << std::endl;
            camera_->ref = user_character->GetGlobalPosition();
            camera_->RotateHorizontallyAboutPoint(0.01f);
            camera_->RecomputeAttributes();
        }

        glm::vec3 temp = user_character->GetGlobalPosition();
        //std::cout << "player position: " << temp.x << ", " << temp.y << ", " << temp.x << std::endl;
        // update viewing elements since camera and user were modified
        //camera_->ref = user_character->GetGlobalPosition();
        camera_->RecomputeAttributes();

        return;
    } else {
        ErrorHandler::ThrowError("on invalid scene");
    }
}

void Scene::Update() {
    //if (playing_) {
        // TODO:: update timer?

        audio_handler_->SetAudioSource(glm::vec3(0.f));
        audio_handler_->Update(user_character->GetGlobalPosition());

        // update view of elements
        if (on_scene_ == SceneList::MAINGAME) {
            glm::mat4 basis(1.f);
            for (std::shared_ptr<Drawable> d : generic_scene_objects_) {
                default_program->Draw(d, basis, this);
            }
            for (std::shared_ptr<Character> d : scene_characters_) {
                default_program->Draw(d->character_body_, d->GetGlobalTransform(), this);
            }
        } else if (on_scene_ == SceneList::START) {
            default_program->Draw(screen_quad_, glm::mat4(1.f), this);
            
        } else if (on_scene_ == SceneList::CONTROLLER) {
            default_program->Draw(screen_quad_, glm::mat4(1.f), this);
        } else {
            ErrorHandler::ThrowError("On invalid Scene");
        }
    //}
}