#include "scene.h"

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
        generic_scene_objects_(vector<shared_ptr<Drawable>>()),
        scene_characters_(vector<shared_ptr<Character>>()) {
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
    shared_ptr<Drawable> main_mesh = static_cast<std::shared_ptr<Drawable>>(new Mesh("resources/wahoo/wahoo.obj", 0));
    user_character = shared_ptr<Character>(new Character(main_mesh, "player", glm::vec3(0.f)));

    //shared_ptr<Drawable> random_mesh = static_cast<std::shared_ptr<Drawable>>(new Mesh("resources/Crate/Crate1.obj", 0));

    GLuint texture = -1;
    //GLuint texture = ShaderProgram::LoadTextureFromBMPFile("resources/wahoo/wahoo.bmp");

    default_program->CreateDrawable(main_mesh, texture);
    //default_program->CreateDrawable(random_mesh, texture);
	//generic_scene_objects_.push_back(main_mesh);
    scene_characters_.push_back(user_character);
}

void Scene::KeyPressEvent(int key, int scancode, int action, int mods) {
	float rot_offset_for_phi_and_theta = 1.f;
	float rot_offset_for_axis = .01f;
    float trans_offset = .1f;

    // TODO: change keypress event handling to a masking so can have multiple keys pressed at once.
    // TODO: make keypress events depend on which camera for direction / movement etc? or have the events pass through the camera as well.
    // ^^ same for mouse move event (bc some cameras will move based on mouse position and some wont).

	// handling rotation checks - phi and theta
	switch (key) {
	/*case GLFW_KEY_UP:
        std::cout << "UP" << std::endl;
        // RotateTheta(rot_offset_for_phi_and_theta
		camera_->RotateAboutPoint(0, rot_offset_for_phi_and_theta);
		break;
	case GLFW_KEY_DOWN:
        std::cout << "DOWN" << std::endl;
		// camera_->RotateTheta(-rot_offset_for_phi_and_theta);
        camera_->RotateAboutPoint(0, -rot_offset_for_phi_and_theta);
		break;
	case GLFW_KEY_LEFT:
        std::cout << "ROT LEFT" << std::endl;
		// camera_->RotatePhi(rot_offset_for_phi_and_theta);
        camera_->RotateAboutPoint(rot_offset_for_phi_and_theta, 0);
		break;
	case GLFW_KEY_RIGHT:
        std::cout << "ROT RIGHT" << std::endl;
		// camera_->RotatePhi(-rot_offset_for_phi_and_theta);
        camera_->RotateAboutPoint(0, -rot_offset_for_phi_and_theta);
		break;
	// handling translational checks*/
	case GLFW_KEY_A:
        std::cout << "MOVE LEFT" << std::endl;
		user_character->MoveLeft();
		break;
	case GLFW_KEY_D:
        std::cout << "MOVE RIGHT" << std::endl;
        user_character->MoveRight();
		break;
	case GLFW_KEY_W:
        std::cout << "MOVE FORWARD" << std::endl;
        user_character->MoveForward();
		break;
	case GLFW_KEY_S:
        std::cout << "MOVE BACKWARD" << std::endl;
        user_character->MoveBackward();
		break;
    case GLFW_KEY_R:
        std::cout << "JUMP" << std::endl;
        user_character->Jump();
        break;
    case GLFW_KEY_F:
        std::cout << "CROUCH" << std::endl;
        user_character->Crouch();
        break;
    default:
        std::cout << key << std::endl;
	}
}

void Scene::ControllerEvents(const unsigned char *button_events, const float *axes_events) {
    // not using switch case so can have multiple at one time
    if (button_events[JoystickButtons::A] == GLFW_PRESS) {
        std::cout << "JUMP" << std::endl;
        user_character->Jump();
    }
    if (button_events[JoystickButtons::B] == GLFW_PRESS) {
        std::cout << "CROUCH" << std::endl;
        user_character->Crouch();
    }
    
    if (axes_events[JoystickAxes::MOVE_X] == -1) {
        std::cout << "MOVE LEFT" << std::endl;
        user_character->MoveLeft();
        camera_->RecomputeAttributes();
    } else if (axes_events[JoystickAxes::MOVE_X] == 1) {
        std::cout << "MOVE RIGHT" << std::endl;
        user_character->MoveRight();
        camera_->RecomputeAttributes();
    }

    if (axes_events[JoystickAxes::MOVE_Y] == 1) {
        std::cout << "MOVE FORWARD" << std::endl;
        user_character->MoveForward();
        camera_->RecomputeAttributes();
    } else if (axes_events[JoystickAxes::MOVE_Y] == -1) {
        std::cout << "MOVE BACKWARD" << std::endl;
        user_character->MoveBackward();
        camera_->RecomputeAttributes();
    }

    if (axes_events[JoystickAxes::CAMROT_Y] == -1) {
        std::cout << "ZOOM IN" << std::endl;
        camera_->TranslateAlongLook(0.5f);
        camera_->RecomputeAttributes();
    } else if (axes_events[JoystickAxes::CAMROT_Y] == 1) {
        std::cout << "ZOOM OUT" << std::endl;
        camera_->TranslateAlongLook(-0.5f);
        camera_->RecomputeAttributes();
    }

    if (axes_events[JoystickAxes::CAMROT_X] == -1) {
        std::cout << "ROT VIEW LEFT" << std::endl;
        camera_->ref = user_character->GetGlobalPosition();
        camera_->RotateAboutPoint(-0.01f);
        camera_->RecomputeAttributes();
    } else if (axes_events[JoystickAxes::CAMROT_X] == 1) {
        std::cout << "ROT VIEW RIGHT" << std::endl;
        camera_->ref = user_character->GetGlobalPosition();
        camera_->RotateAboutPoint(0.01f);
        camera_->RecomputeAttributes();
    }

    glm::vec3 temp = user_character->GetGlobalPosition();
    std::cout << "player position: " << temp.x << ", " << temp.y << ", " << temp.x << std::endl;
    // update viewing elements since camera and user were modified
    //camera_->ref = user_character->GetGlobalPosition();
    camera_->RecomputeAttributes();
}

void Scene::Update() {
   
  
    
    // update view of elements
    glm::mat4 basis(1.f);
    for (std::shared_ptr<Drawable> d : generic_scene_objects_) {
        default_program->Draw(d, basis, this);
    }
    for (std::shared_ptr<Character> d : scene_characters_) {
        default_program->Draw(d->character_body_, d->GetGlobalTransform(), this);
    }
}