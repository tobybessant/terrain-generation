#include "Terrain.h"

Terrain::Terrain(GLuint width, GLuint height, GLfloat tileSize)
{
	std::vector<GLfloat> normals = { 0.0f, 0.1f };
	std::vector<std::vector<GLfloat>> colours = {
		{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.2f, 1.0f, 1.0f }
	};

	for (size_t row = 0; row < width; row++)
	{
		GLfloat rowOffset = row * tileSize;
		for (size_t col = 0; col < height; col++) {

			// position
			vertices.push_back((GLfloat)col * tileSize);
			vertices.push_back(normals[col % 2]);
			vertices.push_back((GLfloat)rowOffset);

			// colour
			std::vector<GLfloat> colour = colours[col % colours.size()];
			// std::vector<GLfloat> colour = colours[1];
			vertices.push_back(colour[0]);
			vertices.push_back(colour[1]);
			vertices.push_back(colour[2]);
		}
	}

	GLuint indexCounter = 0;
	for (size_t index = 0; indices.size() < ((height * width) - width) * 2; index += 2)
	{
		indices.push_back(index);
		indices.push_back(index + width); 
		indices.push_back(index + 1);
		indices.push_back(index + 1 + width);

		indexCounter += 4;
		if (indexCounter == width * 2) {
			// indices.push_back(index + 1 + width);
			// indices.push_back(index + 2 + width);
			indices.push_back(0xFFFF);
			indexCounter = 0;
		}
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*) (sizeof(GLfloat) * 3));

	// center terrain
	model = glm::translate(model, glm::vec3(-((width / 2 ) * tileSize), -1.0f, -((height / 2) * tileSize)));

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

	// Adding all matrices up to create combined matrix
	mvp = projection * view * model;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Terrain::render(GLuint& program)
{
	//model = glm::rotate(model, 0.0001f, glm::vec3(0.0f, 1.0f, 0.0f));
	//mvp = projection * view * model;

	////adding the Uniform to the shader
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}
