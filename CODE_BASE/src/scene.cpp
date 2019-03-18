#include "scene.h"

Scene::Scene()
	:	WindowMaintainer(),
		camera_(new Camera()),
		scene_objects_(vector<shared_ptr<Drawable>>()) {
}
Scene::Scene(Scene& s) {
	scene_objects_ = s.scene_objects();
	*default_program = *(s.default_program);
	camera_ = s.camera();
}

Scene::~Scene() {
	camera_.reset();
	scene_objects_.clear();
}

const glm::mat4& Scene::ViewProjection() const {
    return camera_->GetViewProj();
}
glm::mat4 Scene::ViewProjection() {
    return camera_->GetViewProj();
}

void Scene::RunWithDefaultSetup() {
	default_program = new DefaultProgram(
		"shader_files/threeD.vertexshader",
		"shader_files/threeD.fragmentshader");
    // TODO: ^^^ switch to having inline shader file info (inside the shader itself)
    Mesh* mesh = new Mesh();
    mesh->Load("resources/wahoo/wahoo.obj", 0);
	shared_ptr<Drawable> m = shared_ptr<Drawable>(mesh);

    GLuint texture = -1;
    //GLuint texture = ShaderProgram::LoadTextureFromBMPFile("resources/wahoo/wahoo.bmp");

    default_program->CreateDrawable(m, texture);
	scene_objects_.push_back(m);
}

void Scene::KeyPressEvent(int key, int scancode, int action, int mods) {
	float rot_offset_for_phi_and_theta = 1.f;
	float rot_offset_for_axis = .001f;
    float trans_offset = .01f;

    // TODO: change keypress event handling to a masking so can have multiple keys pressed at once.
    // TODO: make keypress events depend on which camera for direction / movement etc? or have the events pass through the camera as well.
    // ^^ same for mouse move event (bc some cameras will move based on mouse position and some wont).

	// handling rotation checks - phi and theta
	switch (key) {
	case GLFW_KEY_UP:
		camera_->RotateTheta(rot_offset_for_phi_and_theta);
		break;
	case GLFW_KEY_DOWN:
		camera_->RotateTheta(-rot_offset_for_phi_and_theta);
		break;
	case GLFW_KEY_LEFT:
		camera_->RotatePhi(rot_offset_for_phi_and_theta);
		break;
	case GLFW_KEY_RIGHT:
		camera_->RotatePhi(-rot_offset_for_phi_and_theta);
		break;
	// handling rotation checks - each axis
	case GLFW_KEY_I:
		camera_->RotateAboutRight(rot_offset_for_axis);
		break;
	case GLFW_KEY_K:
		camera_->RotateAboutRight(-rot_offset_for_axis);
		break;
	case GLFW_KEY_J:
		camera_->RotateAboutUp(rot_offset_for_axis);
		break;
	case GLFW_KEY_L:
		camera_->RotateAboutUp(-rot_offset_for_axis);
		break;
	// handling translational checks
	case GLFW_KEY_A:
		camera_->TranslateAlongRight(trans_offset);
		break;
	case GLFW_KEY_D:
		camera_->TranslateAlongRight(-trans_offset);
		break;
	case GLFW_KEY_W:
		camera_->TranslateAlongUp(-trans_offset);
		break;
	case GLFW_KEY_S:
		camera_->TranslateAlongUp(trans_offset);
		break;
	case GLFW_KEY_R:
		camera_->TranslateAlongLook(trans_offset);
		break;
	case GLFW_KEY_F:
		camera_->TranslateAlongLook(-trans_offset);
		break;
	}
}

void Scene::Render() {
	for (std::shared_ptr<Drawable> d : scene_objects_) {
		default_program->Draw(d, this);
	}
}