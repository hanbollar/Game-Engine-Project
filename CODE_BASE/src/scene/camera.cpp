#include "camera.h"

Camera::Camera()
	: Camera(SCR_WIDTH, SCR_HEIGHT) {}

Camera::Camera(unsigned int w, unsigned int h)
	: Camera(w, h, glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp)
	: fovy(45.f), width(w), height(h), near_clip(0.1f), far_clip(1000), eye(e), ref(r), world_up(worldUp) {
	
	RecomputeAttributes();
}

Camera::Camera(const Camera &c)
	: fovy(c.fovy), width(c.width), height(c.height), near_clip(c.near_clip), far_clip(c.far_clip), aspect(c.aspect),
	eye(c.eye), ref(c.ref), look(c.look), up(c.up), right(c.right), world_up(c.world_up), V(c.V), H(c.H)
{}

/*************************************/
/****** recalculating functions ******/
/*************************************/

void Camera::RecomputeAttributes() {
	look = glm::normalize(ref - eye);
	right = glm::normalize(glm::cross(look, world_up));
	up = glm::cross(right, look);

	float tan_fovy = glm::tan(fovy / 2.f);
	float len = glm::length(ref - eye);
	aspect = (float)width / height;
	V = up * len*tan_fovy;
	H = right * len*aspect*tan_fovy;
}

void Camera::Resize(unsigned int w, unsigned int h) {
	width = w;
	height = h;
	RecomputeAttributes();
}

void Camera::Reset() {
	fovy = 45.f;
	eye = glm::vec3(0, 0, 10);
	ref = glm::vec3(0, 0, 0);
	world_up = glm::vec3(0, 1, 0);
	RecomputeAttributes();
}

/***************************************/
/****** camera movement functions ******/
/***************************************/

glm::mat4 Camera::GetViewProj() {
	return glm::perspective(fovy, width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::GetViewProj() const {
	return glm::perspective(fovy, width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(eye, ref, up);
}

glm::mat4 Camera::GetProj() {
	return glm::perspective(fovy, width / (float)height, near_clip, far_clip);
}

// TODO: (camera) switch to using quaternions here?

void Camera::RotateAboutUp(float deg) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
	ref = ref - eye;
	ref = glm::vec3(rotation * glm::vec4(ref, 1));
	ref = ref + eye;
	RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
	ref = ref - eye;
	ref = glm::vec3(rotation * glm::vec4(ref, 1));
	ref = ref + eye;
	RecomputeAttributes();
}

void Camera::RotateHorizontallyAboutPoint(float deg) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.f), deg, world_up);
    eye -= ref;
    eye = glm::vec3(rotation * glm::vec4(eye, 1));
    eye += ref;
    RecomputeAttributes();
}

void Camera::RotateVerticallyAboutPoint(float deg) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.f), deg, right);
    eye -= ref;
    eye = glm::vec3(rotation * glm::vec4(eye, 1));
    eye += ref;
    RecomputeAttributes();
}

void Camera::RotateTheta(float deg) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), right);

	eye = eye - ref;
	glm::vec3 old_eye = eye;

	eye = glm::vec3(rotation * glm::vec4(eye, 1));

	if (glm::sign(old_eye[0]) != glm::sign(eye[0]) || glm::sign(old_eye[2]) != glm::sign(eye[2])) {
		eye = old_eye + ref;
	} else {
		eye = eye + ref;
	}

	RecomputeAttributes();
}

void Camera::RotatePhi(float deg) {
	float cos = glm::cos(glm::radians(deg));
	float sin = glm::sin(glm::radians(deg));

	glm::mat4 rotation(glm::vec4(cos, 0.f, -sin, 0.f),
		glm::vec4(0.f, 1.f, 0.f, 0.f),
		glm::vec4(sin, 0.f, cos, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f));

	eye = eye - ref;
	eye = glm::vec3(rotation * glm::vec4(eye, 1));
	eye = eye + ref;
	RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt) {
	glm::vec3 translation = look * amt;
	eye += translation;
	ref += translation;
	RecomputeAttributes();
}

void Camera::TranslateAlongRight(float amt) {
	glm::vec3 translation = right * amt;
	eye += translation;
	ref += translation;
	RecomputeAttributes();
}
void Camera::TranslateAlongUp(float amt) {
	glm::vec3 translation = up * amt;
	eye += translation;
	ref += translation;
	RecomputeAttributes();
}

void Camera::Zoom(float amt) {
	glm::vec3 translation = look * amt;
	eye += translation;
	RecomputeAttributes();
}