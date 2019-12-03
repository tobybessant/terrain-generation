#include "Terrain.h"

Terrain::Terrain(GLuint _width, GLuint _height, GLfloat _tileSize)
{
	width = _width;
	height = _height;
	tileSize = _tileSize;

	generateTerrainData();
	loadTerrainData();
}

void Terrain::render(GLuint& program)
{
	glBindVertexArray(VAO);

	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	// center terrain
	model = glm::translate(model, glm::vec3(-((width - 1) * tileSize) / 2, -0.5f, -1.0f));
	model = glm::rotate(model, -3.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;

	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}

void Terrain::generateTerrainData()
{
	for (size_t row = 0; row < width; row++)
	{
		GLfloat rowOffset = row * tileSize;
		for (size_t col = 0; col < height; col++) {
			vertices.push_back((GLfloat) col * tileSize);
			vertices.push_back(0.0f);
			vertices.push_back((GLfloat) rowOffset);
		}
	}

	GLuint requiredIndexCount = ((height * width) - width) * 2;
	for (size_t index = 0; indices.size() < requiredIndexCount; index += 2)
	{
		indices.push_back(index);
		indices.push_back(index + width);
		indices.push_back(index + 1);
		indices.push_back(index + 1 + width);
	}
}

void Terrain::loadTerrainData() {
	// generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Terrain::centerTerrain() {
}
