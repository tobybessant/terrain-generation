#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "Terrain.h"
#include "LoadShaders.h"

using namespace std;

#define SCREEN_X 1920
#define SCREEN_Y 1080

#define TILE_SIZE 0.1

#define ROWS 40
#define COLS 40

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCREEN_X, SCREEN_Y, "Terrain Generator", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER, "shaders/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	Terrain t = Terrain(COLS, ROWS, TILE_SIZE);

	while (true) {
		glClear(GL_COLOR_BUFFER_BIT);

		t.render(program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
