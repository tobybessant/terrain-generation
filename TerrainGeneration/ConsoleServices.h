#pragma once
#include "Terrain.h"
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

class ConsoleServices
{
public:
	Terrain* askForTerrain();

	Terrain* startTerrainWizard();
	Terrain* loadTerrainFromFile(string& path);

	void askForTerrainPath(string* response);
	void clearConsole();
	void printLine(const vector<string>& segments);
	void exportToFile(std::string& data);
private:
	void askForUnsignedInt(string name, string description, GLuint* response);
	void askForUnsignedInt(string name, string description, GLuint* response, GLuint max);
	void askForFloat(string name, string description, GLfloat* response);
	void askForSeed(string name, string description, string* response);

	bool isNumber(const std::string& s);
	bool isFloat(const std::string& s);
	bool isFile(std::string& path);

	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType, GLfloat& noiseFrequency, string& seed);
	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType, GLfloat& noiseFrequency);
	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType);
	void printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize);
	void printTerrainConfig(GLuint& terrainSize);

	std::string getValue(char* line);
};

