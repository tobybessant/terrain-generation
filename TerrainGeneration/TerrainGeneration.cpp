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

bool isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
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
	cout << "  -=| NEW TERRAIN |=-\n\n" << endl;

	// ask for terrain size
	GLuint terrainSize;
	cout << "  -= Terrain Size =-" << endl;
	cout << "  Single integer value that will be the height & width of the terrain. RECOMMENDED: 500 - 1500" << endl;
	cout << "  ->  ";
	cin >> terrainSize;

	clearConsole();
	printLine({ "  Terrain Size: ", to_string(terrainSize) });
	cout << "\n----------------------\n";

	// as for tile size
	GLfloat tileSize;
	cout << "  -= Tile Size =-" << endl;
	cout << "  Width and height of each tile. RECOMMENDED: 0.01 - 0.2" << endl;
	cout << "  ->  ";
	cin >> tileSize;

	clearConsole();
	printLine({ "  Terrain Size: ", to_string(terrainSize) });
	printLine({ "  Tile Size: ", to_string(tileSize) });
	cout << "\n----------------------\n";

	// ask for nosie type
	GLuint selectedNoiseTypeIndex;
	FastNoise::NoiseType noiseType;
	vector<string> noiseTypes { "Value", "Value Fractal", "Perlin", "Perlin Fractal", "Simplex", "Simplex Fractal", "Cellular", "White Noise", "Cubic", "Cubic Fractal" };

	cout << "  -= Noise Type =-" << endl;
	cout << "  Nose algorithm used to generate the height map." << endl;
	for (int i = 0; i < noiseTypes.size(); i++)
	{
		printLine({ "  [", to_string(i), "] = ", noiseTypes[i] });
	}
	cout << "  Enter desired noise index: ";
	cin >> selectedNoiseTypeIndex;
	noiseType = static_cast<FastNoise::NoiseType>(selectedNoiseTypeIndex);
	clearConsole();

	printLine({ "  Terrain Size: ", to_string(terrainSize) });
	printLine({ "  Tile Size: ", to_string(tileSize) });
	printLine({ "  Noise Type: ", noiseTypes[selectedNoiseTypeIndex] });
	cout << "\n----------------------\n";
	GLfloat noiseFrequency;
	cout << "  -= Noise Frequency =-" << endl;
	cout << "  Frequency of the noise to be generated. RECOMMENDED: 0.01 - 0.1" << endl;
	cout << "  ->  ";
	cin >> noiseFrequency;
	clearConsole();

	printLine({ "   + -= Terrain Config =- " });
	printLine({ "   | Width: ", to_string(terrainSize) });
	printLine({ "   | Height: ", to_string(terrainSize) });
	printLine({ "   | Tile Size: ", to_string(tileSize) });
	printLine({ "   | Noise Type: ", noiseTypes[selectedNoiseTypeIndex] });
	printLine({ "   | Noise Frequency: ", to_string(noiseFrequency) });
	printLine({ "   + -------------------" });

	printLine({ "   | Building terrain. . ." });
	printLine({ "   + ------------------- " });

	return new Terrain(terrainSize, tileSize, noiseType, noiseFrequency);
}

int main() {
	// input and window setup
	InputManager* inputManager = new InputManager();
	GLFWServices glfw = GLFWServices(inputManager);
	glfw.intialiseWindow(SCREEN_X, SCREEN_Y, "Terrain Generator");

	Time* time = new Time();

	glewInit();
	GLuint program = programSetup();
	do {
		// terrain and camera setup
		Terrain* t = askForTerrainConfig();
		Camera cam = Camera(inputManager, time);

		// mvp setup
		glm::mat4 mvp;
		glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9, 0.1f, 20.0f);

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