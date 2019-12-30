#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "Dependencies/FastNoise/FastNoise.h"

#include "GLFWServices.h"
#include "Terrain.h"
#include "Camera.h"
#include "LoadShaders.h"
#include "Time.h"
#include "ConsoleServices.h"

using namespace std;

GLuint programSetup() {
	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0, 2.0);
	
	return program;
}

int main() {
	
	ConsoleServices console = ConsoleServices();

	// input and window setup
	InputManager* inputManager = new InputManager();
	GLFWServices glfw = GLFWServices(inputManager);
	glfw.intialiseWindow("Terrain Generator");

	Time* time = new Time();
	Camera cam = Camera(inputManager, time);
	// mvp setup
	glm::mat4 mvp;
	glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9, 0.1f, 900.0f);

	glewInit();
	GLuint program = programSetup();

	Terrain* t;

	inputManager->addKeyBinding(GLFW_KEY_UP, [&]() { t->increaseMagnitude(); });
	inputManager->addKeyBinding(GLFW_KEY_DOWN, [&]() { t->decreaseMagnitude(); });

	inputManager->addKeyBinding(GLFW_KEY_EQUAL, [&]() { t->increaseNoiseFrequency(); });
	inputManager->addKeyBinding(GLFW_KEY_MINUS, [&]() { t->decreaseNoiseFrequency(); });

	inputManager->addKeyBinding(GLFW_KEY_R, [&]() { t->regenerateTerrain(); });
	do {
		// terrain and camera setup
		t = console.askForTerrain();

		glfw.showWindow();
		while (!glfw.windowShouldClose()) {
			// clear window using buffers
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);

			// update mvp
			mvp = projection * cam.getPosition() * t->getModel();
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

			// render terrain 
			t->render(program);

			// update time and glfw states
			time->update();
			glfw.update();
		}

		glfw.hideWindow();

		delete t;
	} while (true);
}