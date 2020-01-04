#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\fwd.hpp>
#include <glm\ext\matrix_transform.hpp>
#include "InputManager.h"
#include "Observer.h"
#include "Time.h"
#include <iostream>
#include "Terrain.h"

class Camera : public Observer
{
public:
	Camera(InputManager* _inputManager, Time* _time);
	glm::mat4 getPosition();

	void setLocation(glm::vec3 pos);
	void setLocationToTerrainCenter(Terrain* t);

	virtual void notify() override;
private:
	InputManager* inputManager;
	Time* time;

	GLuint currentSpeedIndex = 0;
	std::vector<GLfloat> speeds { 15.0f, 30.0f };

	// cam positions
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// cam cofig
	GLfloat speed = speeds[currentSpeedIndex];
	GLfloat yaw = 0.0f;
	GLfloat pitch = 0.0f;
	GLfloat sensitivity = 0.05;

	// mouse position details
	GLboolean firstMouse = true;

	GLfloat lastX = 0.0f;
	GLfloat lastY = 0.0f;

	void registerKeypressCallbacks();
	void updateCameraPosition(GLdouble mousePosX, GLdouble mousePosY);
	void cycleSpeed();
};

