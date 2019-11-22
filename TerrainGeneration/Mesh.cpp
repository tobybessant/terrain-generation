#include "Mesh.h"

Mesh::Mesh(std::vector<std::vector<GLfloat>> _vertices, std::vector<GLuint> _indices, GLuint* _program) {
	vertices = _vertices;
	indices = _indices;
	program = _program;

	// create and bind VAO buffers
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// generate VBO / EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// load vertex data into buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// bind and load EBO data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	// mvp setup

	// creating the model matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	// creating the view matrix
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));
}

void Mesh::render()
{
	glBindVertexArray(VAO);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;

	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(*program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
