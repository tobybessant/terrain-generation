#pragma once
#define _USE_MATH_DEFINES

#include "Dependencies/FastNoise/FastNoise.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm\fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <math.h> 
#include <cmath>
#include <string>

class Terrain
{
public:
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency);
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency, GLint seed);
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency, GLint seed, GLint noiseOctaves, GLint noiseMagnitude, GLboolean isIsland);

	std::string getTerrainConfigString();

	glm::vec3 getFirstVertexPosition();

	void increaseNoiseFrequency();
	void decreaseNoiseFrequency();

	void increaseMagnitude();
	void decreaseMagnitude();

	void increaseOctaves();
	void decreaseOctaves();

	void makeIsland();
	
	void regenerateTerrain();

	void render(GLuint& program);
	glm::mat4 getModel();
private:
	GLuint  VAO;
	GLuint  VBO, EBO;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	GLfloat fillR = 0.0f;
	GLfloat fillG = 0.0f;
	GLfloat fillB = 0.0f;

	GLuint width;
	GLuint height;

	GLint centerX;
	GLint centerY;
	GLfloat maxDistance;

	GLfloat tileSize;
	GLint seed;
	GLuint octaves;
	GLint magnitude;
	GLfloat exponent = 2;

	GLboolean isIsland = false;

	FastNoise noise;
	GLfloat noiseFrequency;
	FastNoise::NoiseType noiseType;

	std::vector<std::vector<GLfloat>> colours = {
		//  r     g     b
		{ 0.25f, 0.36f, 1.56f, }, // water
		{ 0.49f, 0.72f, 0.45f, }, // land
		{ 0.45f, 0.72f, 0.46f, }, // higher land
		{ 1.0f, 1.0f, 1.0f, } // snow
	};

	glm::mat4 model = glm::mat4(1.0f);

	void setDefaults();
	void calculateMaxDistance();

	void createTerrain();

	void generateVertices();
	void generateIndices();

	void initBuffers();

	void updateColourForHeight(GLuint& startIndex, GLfloat& y);
	void addColourForHeight(GLfloat& y);

	void updateHeightmap(GLboolean useNewSeed);
	
	void loadIntoShader();
};

