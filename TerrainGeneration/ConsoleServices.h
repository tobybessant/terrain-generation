#pragma once
#include "Terrain.h"
#include <string>

using namespace std;

class ConsoleServices
{
public:
	Terrain* askForTerrain();
	void clearConsole();
	void printLine(const vector<string>& segments);

private:
	void askForUnsignedInt(string name, string description, GLuint* response);
	void askForUnsignedInt(string name, string description, GLuint* response, GLuint max);
	void askForFloat(string name, string description, GLfloat* response);
	void askForSeed(string name, string description, string* response);

	bool isNumber(const std::string& s);
	bool isFloat(const std::string& s);

	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType, GLfloat& noiseFrequency, string& seed);
	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType, GLfloat& noiseFrequency);
	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType);
	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize);
	void printTerrainConfig(GLuint& terrainSize);
};

