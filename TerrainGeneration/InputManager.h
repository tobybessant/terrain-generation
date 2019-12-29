#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <functional>
#include <map>
#include <iostream>
#include "Observer.h"
#include <vector>

using Callback = std::function<void()>;

class InputManager
{
public:
	InputManager();
	
	void addKeyBinding(GLuint key, const Callback& callback);
	void trigger(GLuint key, GLuint action);
	void triggerMouseUpdate(GLdouble xpos, GLdouble ypos);

	GLdouble getMouseX();
	GLdouble getMouseY();

	void registerMouseObserver(Observer& o);
private:
	std::map<int, Callback> keypressCallbacks;

	std::vector<std::reference_wrapper<Observer>> mouseObservers;

	GLdouble mouseX;
	GLdouble mouseY;

	void notifyMouseObservers();
};

