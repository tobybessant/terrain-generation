#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Time
{
public:
	GLfloat deltaTime = 0.0f;
	
	void update();
private:
	GLfloat lastFrame = 0.0f;
};

