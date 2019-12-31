#include "Camera.h"

Camera::Camera(InputManager* _inputManager, Time* _time)
{
	time = _time;
	inputManager = _inputManager;
	registerKeypressCallbacks();
	inputManager->registerMouseObserver(*this);
}

glm::mat4 Camera::getPosition()
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::notify()
{
	GLdouble xpos = inputManager->getMouseX();
	GLdouble ypos = inputManager->getMouseY();

	updateCameraPosition(xpos, ypos);
}

void Camera::registerKeypressCallbacks()
{
	inputManager->addKeyBinding(GLFW_KEY_W, [&]() {
		position += forward * speed * time->deltaTime;
	});

	inputManager->addKeyBinding(GLFW_KEY_S, [&]() {
		position -= forward * speed * time->deltaTime;
	});

	inputManager->addKeyBinding(GLFW_KEY_A, [&]() {
		position -= glm::normalize(glm::cross(forward, up)) * speed * time->deltaTime;
	});

	inputManager->addKeyBinding(GLFW_KEY_D, [&]() {
		position += glm::normalize(glm::cross(forward, up)) * speed * time->deltaTime;
	});

	inputManager->addKeyBinding(GLFW_KEY_Q, [&]() {
		cycleSpeed();
	});
}

void Camera::updateCameraPosition(GLdouble mousePosX, GLdouble mousePosY)
{
	if (firstMouse)
	{
		lastX = mousePosX;
		lastY = mousePosY;
		firstMouse = false;
	}

	GLfloat xoffset = mousePosX - lastX;
	GLfloat yoffset = lastY - mousePosY;
	lastX = mousePosX;
	lastY = mousePosY;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(front);
}

void Camera::cycleSpeed()
{
	currentSpeedIndex = ++currentSpeedIndex % speeds.size() == 0 ? 0 : currentSpeedIndex;
	speed = speeds[currentSpeedIndex];
}
