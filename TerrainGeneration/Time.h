#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Time
{
public:
	void update();
	GLfloat getDeltaTime();
private:
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
};

