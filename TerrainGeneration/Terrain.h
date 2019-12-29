#pragma once
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

class Terrain
{
public:
	Terrain(GLuint _size);
	Terrain(GLuint _size, GLfloat _tileSize);
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType);
	Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency);

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

	GLuint width = 1000;
	GLuint height = 1000;
	GLfloat tileSize = 0.02;

	FastNoise noise;
	GLfloat noiseFrequency = 0.02;
	FastNoise::NoiseType noiseType = FastNoise::Perlin;

	glm::mat4 model = glm::mat4(1.0f);

	void createTerrain();

	void generateVertices();
	void generateIndices();
	void loadIntoShader();
};

