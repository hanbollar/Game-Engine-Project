#include "scene.h"
#include "shader_programs/default_program.h"
#include "shader_programs/simple_program.h"

#include <random>

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
        generic_scene_objects_(vector<shared_ptr<SceneObject>>()),
        scene_characters_(vector<shared_ptr<Character>>()),
        audio_handler_(new AudioHandler()),
        text_handler_(new TextHandler()),
        collision_handler_(new CollisionHandler()),
        timer_(shared_ptr<Timer>(new Timer())),
        on_selected_(0)
{}

Scene::Scene(Scene& s)
    :   WindowMaintainer(),
        camera_(s.camera_),
        generic_scene_objects_(s.generic_scene_objects_),
        scene_characters_(s.scene_characters_),
        audio_handler_(std::move(s.audio_handler_)),
        text_handler_(s.text_handler_),
        collision_handler_(std::move(s.collision_handler_)),
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

    blank_quad_ = std::shared_ptr<Drawable>(new Drawable());
    blank_quad_->CreateBlankQuad();

    // screen
    std::shared_ptr<ShaderProgram> simple_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/simple.fragmentshader"));
    screen_quad_ = unique_ptr<SceneObject>(new SceneObject(
        blank_quad_, simple_program, "screenquad", glm::vec3(0.f)));
    screen_quad_->SetTexture(loadingTextures_[0]);
    screen_quad_->SetDrawMode(GL_TRIANGLE_STRIP);
    screen_quad_->CreateSelf();

    screen_quad_->Draw(this->ViewProjection());
}

void Scene::SetupCatLocations() {
    const glm::vec3 MIN_LOC(-50, 0, -50);
    const glm::vec3 MAX_LOC(50, 0, 50);

    for (int i = 0; i < NUMCATS; ++i) {
        cat_loc_[i] = glm::vec3(
            LERP(MIN_LOC.x, MAX_LOC.x, float(rand()) / RAND_MAX),
            LERP(MIN_LOC.y, MAX_LOC.y, float(rand()) / RAND_MAX),
            LERP(MIN_LOC.z, MAX_LOC.z, float(rand()) / RAND_MAX));

        ErrorHandler::ShowError("cat" + to_string(i) + ": "
            + to_string(cat_loc_[i].x) + ", "
            + to_string(cat_loc_[i].y) + ", "
            + to_string(cat_loc_[i].z));
    }
}

void Scene::StartGame() {
    camera_->eye = glm::vec3(10, 10, 10);
    SetupCatLocations();

    /********* FILLING IN BASE SCREEN INFO *******/
    selected_[StartTextures::START_SELECTED] = ShaderProgram::LoadTextureFromFile("resources/Start/1.jpg");
    selected_[StartTextures::CREDITS_SELECTED] = ShaderProgram::LoadTextureFromFile("resources/Start/2.jpg");
    selected_[StartTextures::CONTROLS_SELECTED] = ShaderProgram::LoadTextureFromFile("resources/Start/3.jpg");
    selected_[StartTextures::ON_CREDITS] = ShaderProgram::LoadTextureFromFile("resources/Start/Credits.jpg");
    selected_[StartTextures::ON_CONTROLS] = ShaderProgram::LoadTextureFromFile("resources/Start/base.jpg");

    /********* BUILDING MAIN SCENE SECTION *******/

    std::shared_ptr<ShaderProgram> threeD_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/threeD.vertexshader",
        "shader_files/threeD.fragmentshader"));

    std::shared_ptr<ShaderProgram> general_tex_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/tex.vertexshader",
        "shader_files/tex.fragmentshader"));

    cat_character_ = std::shared_ptr<Character>(new Character(
        "resources/real_cat/cat.obj", Filetype::OBJ, threeD_prog, "real cat", glm::vec3(0.f)
    ));
    cat_character_->CreateSelf();

    fake_cat_character_ = std::shared_ptr<Character>(new Character(
        "resources/real_cat/cat.obj", Filetype::OBJ, threeD_prog, "fake cat", glm::vec3(0.f)));
    //fake_cat_character_->SetTexture(ShaderProgram::LoadTextureFromFile("resources/simple_cat/cat_01_color05.tga"));
    fake_cat_character_->CreateSelf();

    random_mesh = shared_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, general_tex_prog, "random", glm::vec3(0.f)));
    random_mesh->SetTexture(ShaderProgram::LoadTextureFromFile("resources/polka_dots.jpg"));
    random_mesh->CreateSelf();
    random_mesh->SetDrawMode(GL_TRIANGLE_STRIP);
    // for testing:
    //random_mesh->AddGlobalTransform(glm::translate(glm::mat4(1.f), cat_loc_[0]));
    //random_mesh->AddGlobalTransform(glm::translate(glm::mat4(1.f), cat_loc_[1]));
    //random_mesh->AddGlobalTransform(glm::translate(glm::mat4(1.f), cat_loc_[2]));

    staff = shared_ptr<SceneObject>(new SceneObject(
        "resources/quad.obj", Filetype::OBJ, general_tex_prog, "random", glm::vec3(0.f)));
    staff->SetTexture(ShaderProgram::LoadTextureFromFile("resources/polka_dots.jpg"));
    staff->SetGlobalTransform(glm::translate(glm::scale(glm::mat4(1.f), 6.f*glm::vec3(.2, 2, .2)), glm::vec3(0, -1, -4.5)));
    staff->CreateSelf();
    staff->SetDrawMode(GL_TRIANGLE_STRIP);

    std::shared_ptr<ShaderProgram> rain_prog_ = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/threeD.vertexshader",
        "shader_files/rain.fragmentshader"));
    rain_prog_->SetTimer(timer_);
    rain_prog_->SetResolution(vec2(800, 800));

    user_character_ = shared_ptr<Character>(new Character(
        "resources/mainchar/walking_char_sequence.dae", Filetype::COLLADA, threeD_prog, "player", glm::vec3(1, 1, 0.f)));
    user_character_->SetGlobalTransform(glm::translate(glm::scale(glm::mat4(1.f), glm::vec3(0.1)), glm::vec3(0, 21, 0)));
    user_character_->CreateSelf();
    
    shared_ptr<SceneObject> rain_floor = shared_ptr<SceneObject>(new SceneObject(
        blank_quad_, rain_prog_, "rain floor", glm::vec3(0.f)));
    float magnitude = 100.f;
    rain_floor->SetGlobalTransform(
        glm::rotate(glm::scale(glm::mat4(1.f), glm::vec3(magnitude)), float(M_PI/2.f), glm::vec3(1, 0, 0)));
    rain_floor->CreateSelf();

    shared_ptr<SceneObject> tree = shared_ptr<SceneObject>(new SceneObject(
        "resources/lowpoly_tree/Lowpoly_tree_sample.dae", Filetype::COLLADA, threeD_prog, "tree", glm::vec3(0, 0, 2)));
    tree->SetDrawMode(GL_TRIANGLE_STRIP);
    tree->CreateSelf();
    
    std::shared_ptr<ShaderProgram> grass_tex_prog = std::shared_ptr<DefaultProgram>(new DefaultProgram(
        "shader_files/grass_tex.vertexshader",
        "shader_files/grass_tex.fragmentshader"));
    shared_ptr<SceneObject> grass = shared_ptr<SceneObject>(new SceneObject(
        "resources/grass/grass_low_poly.DAE", Filetype::COLLADA, grass_tex_prog, "grass", glm::vec3(0, 0, 2)));
    grass->SetTextures(ShaderProgram::LoadTextureFromFile("resources/grass/low_poly_grass2_fix.png"),
                       ShaderProgram::LoadTextureFromFile("resources/grass/low_poly_grass_opacity.png"));
    grass->CreateSelf();
    
    std::shared_ptr<ShaderProgram> sky_program = std::shared_ptr<SimpleProgram>(new SimpleProgram(
        "shader_files/simple.vertexshader",
        "shader_files/sky.fragmentshader"));
    sky_quad_ = unique_ptr<SceneObject>(new SceneObject(blank_quad_, sky_program, "skyquad", glm::vec3(0.f)));
    sky_quad_->CreateSelf();

    ErrorHandler::PrintGLErrorLog();

    /******** CONNECTING OBJECTS AND THEIR SUBS ******/

    user_character_->attached_components_.push_back(staff);


    /******** SETTING UP FOR COLLISION CHECKS ******/

    glm::mat4 transf_1 = glm::translate(glm::mat4(1.f), glm::vec3(-5, 0, -7));
    glm::mat4 transf_2 = glm::translate(glm::mat4(1.f), glm::vec3(5, 0, 5));
    std::vector<Primitive*> prims = {
        new Primitive(transf_1, ShapeType::CUBE),
        new Primitive(transf_2, ShapeType::CUBE) };

    collision_handler_->Build(prims);

    // for visualizing the collisions for now
    std::shared_ptr<ShaderProgram> collis_prog = std::shared_ptr<ShaderProgram>(new DefaultProgram(
        "shader_files/threeD.vertexshader",
        "shader_files/threeD_collis.fragmentshader"
    ));
    std::shared_ptr<SceneObject> collision_cubes = std::shared_ptr<SceneObject>(new SceneObject(
        "resources/dodecahedron.obj", Filetype::OBJ, collis_prog, "collision", glm::vec3(0)
    ));
    collision_cubes->CreateSelf();
    collision_cubes->SetDrawMode(GL_TRIANGLE_STRIP);
    collision_cubes->AddGlobalTransform(transf_1);
    collision_cubes->AddGlobalTransform(transf_2);

    /****** ADDING AS RENDERABLES *******/

    // adding in scene objects that are opaque
    scene_characters_.push_back(user_character_);
	
    generic_scene_objects_.push_back(rain_floor);
    RandomlyAddTransformations(tree, 10, glm::vec3(.5, .5, .5), glm::vec3(1.5, 2, 1.5), glm::ivec3(50, 1, 50), glm::vec3(10, 1, 10));
    generic_scene_objects_.push_back(tree);
    //generic_scene_objects_.push_back(staff);

    // adding in collision object visuals
    generic_scene_objects_.push_back(collision_cubes);

    // adding in scene objects that have opacity
    RandomlyAddTransformations(grass, 500, glm::vec3(0.2), glm::vec3(1.f), glm::ivec3(100, 1, 100), glm::vec3(0));
    generic_scene_objects_.push_back(grass);

    /****** SETTING UP SCREEN TEXT *******/

    screen_quad_->SetTexture(StartTextures::START_SELECTED);

    text_handler_->initText2D(10, 570, 15, "timer", TextId::TIMER);
    text_handler_->initText2D(5, 5, 20, "general info", TextId::GENERALSCREEN);


}

void Scene::RandomlyAddTransformations( std::shared_ptr<SceneObject> scene_obj, const unsigned int& num,
                                const glm::fvec3& min_scale, const glm::fvec3&max_scale,
                                const glm::ivec3& rand_dim, const glm::vec3& center) {
    glm::vec3 z(0, 1, 0);
    glm::mat4 identity(1.f);
    glm::mat4 m_max_scale(glm::scale(identity, max_scale));
    glm::mat4 m_min_scale(glm::scale(identity, min_scale));

    glm::vec3 calc_center = center - glm::vec3(rand_dim / 2);

    for (int i = 0; i < num; ++i) {
        glm::mat4 trans = glm::translate(
            identity,
            calc_center + glm::vec3(rand() % rand_dim.x, rand() % rand_dim.y, rand() % rand_dim.z)
        );
        glm::mat4 rot = glm::rotate(identity, float(M_PI) / ((rand() % 180) + 1), z);
        glm::mat4 scale = LERP(m_min_scale, m_max_scale, rand() / RAND_MAX);
        glm::mat4 transformation = trans * rot * scale;
        scene_obj->AddGlobalTransform(transformation);
    }
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
                    audio_handler_->StartPlayingSound(AudioChoices::CAT1, AudioChoices::GENERAL);
                    audio_handler_->SetAudioSourcePos(cat_loc_[on_cat_]);
                    audio_handler_->SwitchToStored2DSound();
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
            if (game_started_ && !playing_) {
                // finished the game and these were the end credits
                ErrorHandler::ShowError("Closing Window. Game Finished.");
                CloseWindow();
                return;
            }

            // otherwise this is first time starting the game on title screen
            on_scene_ = SceneList::START;
            on_selected_ = StartTextures::START_SELECTED;
        }

        break;
    case SceneList::CONTROLS:
        if (button_events[JoystickButtons::B] == GLFW_PRESS) {
            if (game_started_) {
                on_scene_ = SceneList::MAINGAME;
                timer_->Play();
                text_handler_->writeToText2D("MAIN GAME", TextId::GENERALSCREEN);
                playing_ = true;
            }
            else {
                on_scene_ = SceneList::START;
                on_selected_ = StartTextures::START_SELECTED;
            }
        }

        break;
    case SceneList::MAINGAME:
        if (button_events[JoystickButtons::A] == GLFW_PRESS) {
            if (cat_in_min_dist_) {
                if (on_cat_ != NUMCATS - 1) {
                    text_handler_->writeToText2D("HMM THAT DOESNT SEEM TO BE MY CAT", TextId::GENERALSCREEN);
                    on_cat_++;
                    cat_in_min_dist_ = false;
                    cat_in_sound_dist_ = false;
                    audio_handler_->SetAudioSourcePos(cat_loc_[on_cat_]);
                    audio_handler_->SwitchToStored2DSound();
                }
                else if (on_cat_ == NUMCATS - 1) {
                    text_handler_->writeToText2D("YOU DID IT!!", TextId::GENERALSCREEN);
                    on_scene_ = SceneList::CREDITS;
                    playing_ = false;
                    audio_handler_->StopPlayingSound();
                }
            }
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
            user_character_->TurnInDirection(true);
        }
        else if (axes_events[JoystickAxes::MOVE_X] == 1) {
            user_character_->TurnInDirection(false);
        }

        if (axes_events[JoystickAxes::MOVE_Y] == 1) {
            if (collision_handler_->TestLoc(user_character_->MoveInDirection(camera_, glm::vec3(0, 0, -1)))) {
                // do nothing
            } else {
                //user_character_->MoveInDirection(camera_, glm::vec3(0, 0, 1)); // COLLISION CHECKS DONT WORK ATM
            }
        } else if (axes_events[JoystickAxes::MOVE_Y] == -1) {
            if (collision_handler_->TestLoc(user_character_->MoveInDirection(camera_, glm::vec3(0, 0, 1)))) {
                // do nothing
            } else {
                // move back
                //user_character_->MoveInDirection(camera_, glm::vec3(0, 0, -1)); // COLLISION CHECKS DONT WORK ATM
            }
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

void Scene::CloseWindow() {
    window_still_running_ = false;
}

void Scene::HandleCatLocationCheck() {

    float dist = glm::distance(user_character_->GetGlobalPosition(), cat_loc_[on_cat_]);

    if (dist > CAT_TOO_FAR_DIST_) {
        //text_handler_->writeToText2D("YOU'RE TOO FAR! TURN BACK!", TextId::GENERALSCREEN);
        return;
    }

    if (dist < CAT_ALMOST_FOUND_MIN_DIST_) {
        if (cat_in_min_dist_) {
            // still valid
        } else {
            // just started beind valid
            cat_in_min_dist_ = true;
            text_handler_->writeToText2D("PRESS A WHEN YOU SEE THE CAT!", TextId::GENERALSCREEN);
        }
    } else {
        // not valid
        cat_in_min_dist_ = false;
    }
    
    if (dist < CAT_SOUND_MIN_DIST_) {
        if (cat_in_sound_dist_) {
            // still valid
            if (!cat_in_min_dist_) {
                text_handler_->writeToText2D("YOU'RE CLOSE! KEEP LOOKING!", TextId::GENERALSCREEN);
            }
        } else {
            // just started being valid
            audio_handler_->SwitchToStored3DSound();
            cat_in_sound_dist_ = true;
        }
    } else {
        // not valid
        if (cat_in_sound_dist_) {
            // just stopped being valid
            audio_handler_->SwitchToStored2DSound();
            cat_in_sound_dist_ = false;
        }
        text_handler_->writeToText2D("", TextId::GENERALSCREEN);
    }
}

void Scene::HandleTimeBasedMovement() {
    // Staff Update
    // right now hard-coded but for future can do this through a function pointer for every
    // scene object attached to a specific scene object (and iterate recursively that way)
    glm::mat4 base_transf = glm::translate(glm::scale(glm::mat4(1.f), 6.f*glm::vec3(.2, 2, .2)), glm::vec3(0, -1, -4.5)); 
    glm::mat4 rotate_to_side_transf = glm::rotate(glm::mat4(1.f), float(M_PI / 2), glm::vec3(0, 1, 0));
        
    float y_loc = std::fabs(glm::cos(timer_->GetTime() * M_PI / 16));
    glm::mat4 time_transf = glm::translate(rotate_to_side_transf * base_transf, glm::vec3(0, .75*y_loc, 0));
    staff->SetGlobalTransform(time_transf);

    // IK Update based on staff
}

void Scene::Update() {
    if (playing_) {
        HandleCatLocationCheck();
        audio_handler_->Update(user_character_->GetGlobalPosition());

        HandleTimeBasedMovement();

        timer_->Tick();
    }

    glm::mat4 vproj = this->ViewProjection();

    switch (on_scene_)
    {
    case SceneList::MAINGAME:
        // sky background
        glDisable(GL_DEPTH_TEST);
        sky_quad_->Draw(this->ViewProjection());
        glEnable(GL_DEPTH_TEST);

        // main scene attributes
        for (auto& so : generic_scene_objects_) {
            so->Draw(vproj);
        }
        for (auto& c : scene_characters_) {
            c->Draw(vproj);
        }

        if (cat_in_min_dist_) {
            /*if (on_cat_ == 3) {
                cat_character_->SetGlobalTransform(glm::translate(glm::mat4(1.f), cat_loc_[on_cat_]));
                cat_character_->Draw(vproj);
            } else {
                fake_cat_character_->SetGlobalTransform(glm::translate(glm::mat4(1.f), cat_loc_[on_cat_]));
                fake_cat_character_->Draw(vproj);
            }*/
            random_mesh->SetGlobalTransform(glm::translate(glm::mat4(1.f), cat_loc_[on_cat_]));
            random_mesh->Draw(vproj);
        }

        // text overlay on screen
        glDisable(GL_DEPTH_TEST);
        text_handler_->writeToText2D(std::to_string(timer_->GetTime()), TextId::TIMER);

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