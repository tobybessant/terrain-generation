#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm\ext\matrix_transform.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\fwd.hpp>
#include <glm\gtc\type_ptr.hpp>

class Mesh
{
public:
	Mesh(std::vector<std::vector<GLfloat>> _vertices, std::vector<GLuint> _indices, GLuint* program);

	void render();

private: 
	GLuint VAO, VBO, EBO;

	GLuint* program;

	std::vector<std::vector<GLfloat>> vertices;
	std::vector<GLuint> indices;

	// blank model and view matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	// default projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 900.0f);
};

