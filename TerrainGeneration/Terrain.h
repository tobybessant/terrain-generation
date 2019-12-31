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

class Terrain
{
public:
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency);
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency, GLuint seed);

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
	GLuint seed;
	GLuint octaves;
	GLint magnitude;

	FastNoise noise;
	GLfloat noiseFrequency;
	FastNoise::NoiseType noiseType;

	glm::mat4 model = glm::mat4(1.0f);

	void createTerrain();

	void generateVertices();
	void generateIndices();

	void addColourForHeight(GLfloat& y);

	void updateHeightmap(GLboolean useNewSeed);
	
	void loadIntoShader();
};

