#include "GLFWServices.h"

void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseHandler(GLFWwindow* window, double xpos, double ypos);

GLFWServices::GLFWServices(InputManager* _inputManager)
{
	inputManager = _inputManager;
	glfwInit();

	registerKeypressCallbacks();
}

void GLFWServices::intialiseWindow(const char* name)
{
	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	window = glfwCreateWindow(mode->width, mode->height, name, NULL, NULL);

	glfwSetWindowUserPointer(window, inputManager);

	glfwSetKeyCallback(window, keypressHandler);
	glfwSetCursorPosCallback(window, mouseHandler);

	glfwSetWindowPos(window, 0, 0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	hideWindow();
}

void GLFWServices::destroyWindow()
{
	glfwDestroyWindow(window);
}

void GLFWServices::hideWindow()
{
	glfwHideWindow(window);
}

void GLFWServices::showWindow()
{
	setWindowShouldClose(false);
	glfwShowWindow(window);
}

bool GLFWServices::windowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void GLFWServices::update()
{
	// swap buffers and poll event states
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GLFWServices::setWindowShouldClose(GLboolean shouldClose)
{
	glfwSetWindowShouldClose(window, shouldClose);
}

void GLFWServices::registerKeypressCallbacks()
{
	inputManager->addKeyBinding(GLFW_KEY_ESCAPE, [&]() {
		setWindowShouldClose(true);
	});
}

void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// forward event onto inputManager object assigned to the window's user pointer
	InputManager* inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->trigger(key, action);
}

void mouseHandler(GLFWwindow* window, double xpos, double ypos)
{
	// forward event onto inputManager object assigned to the window's user pointer
	InputManager* inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->triggerMouseUpdate(xpos, ypos);
}