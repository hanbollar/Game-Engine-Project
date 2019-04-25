#include "scene.h"
#include "shader_programs/default_program.h"
#include "shader_programs/simple_program.h"

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
        generic_scene_objects_(vector<shared_ptr<SceneObject>>()),
        scene_characters_(vector<shared_ptr<Character>>()),
        audio_handler_(new AudioHandler()),
        text_handler_(new TextHandler()),
        timer_(shared_ptr<Timer>(new Timer())),
        on_selected_(0)
{}

Scene::Scene(Scene& s)
    :   WindowMaintainer(),
        camera_(s.camera_),
        generic_scene_objects_(s.generic_scene_objects_),
        scene_characters_(s.scene_characters_),
        audio_handler_(s.audio_handler_),
        text_handler_(s.text_handler_),
        timer_(s.timer_),
        on_selected_(s.on_selected_)
{}

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
    RunLoadingScreen();
 
    StartGame();
}

void Scene::RunLoadingScreen() {
    loadingTextures_[0] = ShaderProgram::LoadTextureFromFile("resources/Load/1.jpg");
    loadingTextures_[1] = ShaderProgram::LoadTextureFromFile("resources/Load/2.jpg");
    loadingTextures_[2] = ShaderProgram::LoadTextureFromFile("resources/Load/3.jpg");
    timer_->Play();

    // screen
    std::shared_ptr<ShaderProgram> simple_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/simple.fragmentshader"));
    screen_quad_ = unique_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, simple_program, "screenquad", glm::vec3(0.f)));
    screen_quad_->SetTexture(loadingTextures_[0]);
    screen_quad_->CreateSelf();

    screen_quad_->Draw(this->ViewProjection());
}

void Scene::StartGame() {
    // filling in base screen info
    selected_[StartTextures::START_SELECTED] = ShaderProgram::LoadTextureFromFile("resources/Start/1.jpg");
    selected_[StartTextures::CREDITS_SELECTED] = ShaderProgram::LoadTextureFromFile("resources/Start/2.jpg");
    selected_[StartTextures::CONTROLS_SELECTED] = ShaderProgram::LoadTextureFromFile("resources/Start/3.jpg");
    selected_[StartTextures::ON_CREDITS] = ShaderProgram::LoadTextureFromFile("resources/Start/Credits.jpg");
    selected_[StartTextures::ON_CONTROLS] = ShaderProgram::LoadTextureFromFile("resources/Start/base.jpg");

    std::shared_ptr<ShaderProgram> threeD_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/threeD.vertexshader",
        "shader_files/threeD.fragmentshader"));

    std::shared_ptr<ShaderProgram> general_tex_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/tex.vertexshader",
        "shader_files/tex.fragmentshader"));

    // SceneObjects
    user_character_ = shared_ptr<Character>(new Character(
        "resources/quad.obj", Filetype::OBJ, general_tex_prog, "player", glm::vec3(0.f)));
    user_character_->SetTexture(ShaderProgram::LoadTextureFromFile("resources/polka_dots.jpg"));
    user_character_->CreateSelf();

    std::shared_ptr<ShaderProgram> rain_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/threeD.vertexshader",
        "shader_files/rain.fragmentshader"));
    rain_prog->SetTimer(timer_);
    rain_prog->SetResolution(vec2(800, 800));

    shared_ptr<SceneObject> random_mesh = shared_ptr<SceneObject>(new SceneObject(
        "resources/mainchar/walking_char_sequence.dae", Filetype::COLLADA, rain_prog, "random", glm::vec3(1, 1, 0.f)));
    random_mesh->CreateSelf();
    
    
    shared_ptr<SceneObject> rain_floor = shared_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, rain_prog, "rain floor", glm::vec3(0.f)));
    float magnitude = 100.f;
    rain_floor->SetGlobalTransform(
        glm::translate(glm::mat4(1.f), 0.5f * glm::vec3(-magnitude, 0, -magnitude))
        * glm::rotate(glm::scale(glm::mat4(1.f), glm::vec3(magnitude)), float(M_PI/2.f), glm::vec3(1, 0, 0)));
    rain_floor->CreateSelf();

    // sky
    std::shared_ptr<ShaderProgram> sky_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/sky.fragmentshader"));
    sky_quad_ = unique_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, sky_program, "skyquad", glm::vec3(0.f)));
    sky_quad_->CreateSelf();

    ErrorHandler::PrintGLErrorLog();

    //user_character_->Attach(staff, );

    scene_characters_.push_back(user_character_);
	generic_scene_objects_.push_back(random_mesh);
    generic_scene_objects_.push_back(rain_floor);

    screen_quad_->SetTexture(StartTextures::START_SELECTED);

    text_handler_->initText2D(10, 570, 15, "timer", TextId::TIMER);
    text_handler_->initText2D(5, 5, 20, "general info", TextId::GENERALSCREEN);
}

void Scene::KeyPressEvent(int key, int scancode, int action, int mods) {
	// no longer using this but keeping here just in case for future.
}

void Scene::ControllerEvents(const unsigned char *button_events, const float *axes_events) {
    int t = timer_->GetTime() * 10;

    switch (on_scene_)
    {
    case SceneList::START:
        
        if (t % 5 != 0) {
            //std::cout << "time: " << t << std::endl;
            // delay for button pressing
            return;
        }
        if (button_events[JoystickButtons::A] == GLFW_PRESS) {
            if (on_selected_ == StartTextures::START_SELECTED) {
                on_scene_ = SceneList::MAINGAME;
                if (!game_started_) {
                    audio_handler_->StartPlayingSound();
                    audio_handler_->SetAudioSourcePos(glm::vec3(0.f));
                    playing_ = true;
                    game_started_ = true;
                }
            } else if (on_selected_ == StartTextures::CONTROLS_SELECTED) {
                on_scene_ = SceneList::CONTROLS;
            } else if (on_selected_ == StartTextures::CREDITS_SELECTED) {
                on_scene_ = SceneList::CREDITS;
            }

            audio_handler_->PlaySingleSound(AudioChoices::BELL);
        }

        if (abs(axes_events[JoystickAxes::MOVE_Y]) > .6) {
            on_selected_ += glm::sign(axes_events[JoystickAxes::MOVE_Y]) * 1;
            on_selected_ = glm::clamp(on_selected_, 1 * StartTextures::START_SELECTED, 1 * StartTextures::CONTROLS_SELECTED);
        }
        

        break;
    case SceneList::CREDITS:
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            on_scene_ = SceneList::START;
            on_selected_ = StartTextures::START_SELECTED;
        }
        
        break;
    case SceneList::CONTROLS:
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            if (game_started_) {
                on_scene_ = SceneList::MAINGAME;
                timer_->Play();
                playing_ = true;
            } else {
                on_scene_ = SceneList::START;
                on_selected_ = StartTextures::START_SELECTED;
            }
        }

        break;
    case SceneList::MAINGAME:
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
            on_scene_ = CONTROLS;
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
            camera_->RotateVerticallyAboutPoint(-0.1f);
        } else if (axes_events[JoystickAxes::CAMROT_Y] == 1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateVerticallyAboutPoint(0.1f);
        }

        if (axes_events[JoystickAxes::CAMROT_X] == -1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateHorizontallyAboutPoint(-0.1f);
        } else if (axes_events[JoystickAxes::CAMROT_X] == 1) {
            camera_->ref = user_character_->GetGlobalPosition();
            camera_->RotateHorizontallyAboutPoint(0.1f);
        }

        // update viewing elements since camera and user were modified
        camera_->RecomputeAttributes();

        break;
    default:
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
        // sky background
        glDisable(GL_DEPTH_TEST);
        sky_quad_->Draw(this->ViewProjection());
        glEnable(GL_DEPTH_TEST);

        // main scene attributes
        for (auto& so : generic_scene_objects_) {
            so->Draw(this->ViewProjection());
        }
        for (auto& c : scene_characters_) {
            c->Draw(this->ViewProjection());
        }

        // text overlay on screen
        glDisable(GL_DEPTH_TEST);
        text_handler_->writeToText2D(std::to_string(timer_->GetTime()), TextId::TIMER);
        text_handler_->writeToText2D("MAIN GAME", TextId::GENERALSCREEN);

        text_handler_->Draw();
        glEnable(GL_DEPTH_TEST);

        break;
    case SceneList::START:
        if (on_selected_ <= NUM_TEXTURES) {
            screen_quad_->SetTexture(selected_[on_selected_]);
        }
        screen_quad_->Draw();
        break;
    case SceneList::CONTROLS:
        if (on_selected_ <= NUM_TEXTURES) {
            screen_quad_->SetTexture(selected_[StartTextures::ON_CONTROLS]);
        }
        screen_quad_->Draw();

        glDisable(GL_DEPTH_TEST);
        text_handler_->writeToText2D("Press B to Return", TextId::GENERALSCREEN);
        text_handler_->Draw();
        glEnable(GL_DEPTH_TEST);

        break;
    case SceneList::CREDITS:
        if (on_selected_ <= NUM_TEXTURES) {
            screen_quad_->SetTexture(selected_[StartTextures::ON_CREDITS]);
        }
        screen_quad_->Draw();

        glDisable(GL_DEPTH_TEST);
        text_handler_->writeToText2D("Press B to Return", TextId::GENERALSCREEN);
        text_handler_->Draw();
        glEnable(GL_DEPTH_TEST);

        break;
    default: 
        ErrorHandler::ThrowError("On invalid Scene");
    }
}