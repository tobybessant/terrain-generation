#include "InputManager.h"

void InputManager::addKeyBinding(GLuint key, const Callback& callback)
{
	keypressCallbacks[key] = callback;
}

void InputManager::trigger(GLuint key, GLuint action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (keypressCallbacks.count(key) > 0) {
			keypressCallbacks[key]();
		}
	}
}

void InputManager::triggerMouseUpdate(GLdouble xpos, GLdouble ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	notifyMouseObservers();
}

GLdouble InputManager::getMouseX()
{
	return mouseX;
}

GLdouble InputManager::getMouseY()
{
	return mouseY;
}

void InputManager::registerMouseObserver(Observer& o)
{
	mouseObservers.push_back(o);
}

void InputManager::notifyMouseObservers()
{
	for (Observer& o : mouseObservers) {
		o.notify();
	}
}

