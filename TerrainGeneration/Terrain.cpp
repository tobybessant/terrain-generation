#include "Terrain.h"

Terrain::Terrain(GLuint width, GLuint height, GLfloat tileSize)
{
	for (size_t row = 0; row < width; row++)
	{
		GLfloat rowOffset = row * tileSize;
		for (size_t col = 0; col < height; col++) {
			vertices.push_back((GLfloat)col * tileSize);
			vertices.push_back(0.0f);
			vertices.push_back((GLfloat)rowOffset);
		}
	}

	for (size_t index = 0; indices.size() < ((height * width) - width) * 2; index += 2)
	{
		indices.push_back(index);
		indices.push_back(index + width);
		indices.push_back(index + 1);
		indices.push_back(index + 1 + width);
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	// center terrain
	model = glm::translate(model, glm::vec3(-((width - 1) * tileSize) / 2, -0.5f, -1.0f));
	model = glm::rotate(model, -3.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	// Adding all matrices up to create combined matrix
	mvp = projection * view * model;


	glEnableVertexAttribArray(0);
}

void Terrain::render(GLuint& program)
{
	model = glm::rotate(model, 0.0001f, glm::vec3(0.0f, 1.0f, 0.0f));
	mvp = projection * view * model;

	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}
