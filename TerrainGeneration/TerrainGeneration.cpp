#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "Dependencies/FastNoise/FastNoise.h"

#include "Terrain.h"
#include "LoadShaders.h"

using namespace std;

#define SCREEN_X 1920
#define SCREEN_Y 1080

#define TILE_SIZE 0.2

#define ROWS 1000
#define COLS 1000

void printLine(const vector<string>& segments) {
	for (size_t i = 0; i < segments.size(); i++)
	{
		cout << segments[i] << " ";
	}
	cout << endl;
}

void clearConsole() {
	for (size_t i = 0; i < 30; i++)
	{
		cout << endl;
	}
}

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

Terrain* askForTerrainConfig() {
	clearConsole();

	// ask for terrain size
	GLuint terrainSize;
	printLine({ "Please enter the size of the terrain (single integer value that will be both H and W)" });
	printLine({ "DEFAULT: 1000 | RECCOMMENDED: 500 - 1500" });
	cin >> terrainSize;
	clearConsole();

	// as for tile size
	GLfloat tileSize;
	printLine({ "Please enter the tile size (single float value between 0 and 1)" });
	printLine({ "DEFAULT: 0.02 | RECCOMMENDED: 0.01 - 0.3" });
	cin >> tileSize;
	clearConsole();

	// ask for nosie type
	GLuint selectedNoiseTypeIndex;
	FastNoise::NoiseType noiseType;
	vector<string> noiseTypes { "Value", "Value Fractal", "Perlin", "Perlin Fractal", "Simplex", "Simplex Fractal", "Cellular", "White Noise", "Cubic", "Cubic Fractal" };

	printLine({ "Please select the type of noise to generate the terrain height map" });
	printLine({ "DEFAULT: 0 (Value) " });
	for (int i = 0; i < noiseTypes.size(); i++)
	{
		printLine({ "[", to_string(i), "] = ", noiseTypes[i] });
	}
	cin >> selectedNoiseTypeIndex;
	noiseType = static_cast<FastNoise::NoiseType>(selectedNoiseTypeIndex);
	clearConsole();

	GLfloat noiseFrequency;
	printLine({ "Please enter the noise frequency" });
	printLine({ "DEFAULT: 0.02 | RECCOMMENDED: 0.01 - 0.1" });
	cin >> noiseFrequency;
	clearConsole();

	return new Terrain(terrainSize, terrainSize, tileSize, noiseType, noiseFrequency);
}

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(SCREEN_X, SCREEN_Y, "Terrain Generator", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glewInit();
	GLuint program = programSetup();

	Terrain* t = askForTerrainConfig();

	while (true) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		t->render(program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
