#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "Mesh.h"
#include "LoadShaders.h"

using namespace std;

#define SCREEN_X 800
#define SCREEN_Y 600

#define MAX_ROWS 5
#define MAX_COLS 5

int main() {
	
	vector<vector<GLfloat>> vertices;
	vector<GLuint> indices;

	// gen vertices
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLS; col++) {
			vector<GLfloat> position;
			position.push_back((GLfloat)col);
			position.push_back(1.0f);
			position.push_back((GLfloat) row);

			vertices.push_back(position);
		}
	}

	// gen indices
	for (int row = 0; row < MAX_ROWS; row++) {
		for (int col = 0; col < MAX_COLS; col++)
		{
			// tri 1
			indices.push_back(col);
			indices.push_back(col + 1);
			indices.push_back(col + MAX_COLS);

			// tri 2
			indices.push_back(col + MAX_COLS);
			indices.push_back(col + 1);
			indices.push_back(col + MAX_COLS);
		}
	}

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
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	Mesh terrain = Mesh(vertices, indices, &program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (true) {
		glClear(GL_COLOR_BUFFER_BIT);

		terrain.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
