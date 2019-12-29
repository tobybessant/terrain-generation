#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "InputManager.h"

class GLFWServices
{
public:
	GLFWServices(InputManager* _inputManager);

	void intialiseWindow(const GLuint width, const GLuint height, const char* name);
	void hideWindow();
	void showWindow();
	bool windowShouldClose();

	void update();

private:
	InputManager* inputManager;
	GLFWwindow* window;

	void setWindowShouldClose(GLboolean shouldClose);
	void registerKeypressCallbacks();
};

