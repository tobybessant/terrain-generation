#include "Terrain.h"

Terrain::Terrain(GLuint _size)
{
	width = _size;
	height = _size;

	createTerrain();
}

Terrain::Terrain(GLuint _size, GLfloat _tileSize)
{
	width = _size;
	height = _size;
	tileSize = _tileSize;

	createTerrain();
}

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType)
{
	width = _size;
	height = _size;
	tileSize = _tileSize;
	noiseType = _noiseType;

	createTerrain();
}

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency)
{
	width = _size;
	height = _size;
	tileSize = _tileSize;
	noiseType = _noiseType;
	noiseFrequency = _noiseFrequency;
}

Terrain::Terrain(GLuint _width, GLuint _height, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency)
{
	width = _width;
	height = _height;
	tileSize = _tileSize;
	noiseType = _noiseType;
	noiseFrequency = _noiseFrequency;

	createTerrain();
}

void Terrain::render(GLuint& program)
{
	model = glm::rotate(model, 0.0001f, glm::vec3(0.0f, 1.0f, 0.0f));
	mvp = projection * view * model;	
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(VAO);

	// draw fill colour
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)(sizeof(GLfloat) * 6));
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);

	// draw polygon colour
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)(sizeof(GLfloat) * 3));
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}

void Terrain::createTerrain()
{
	printConfig();
	generateVertices();
	generateIndices();
	loadIntoShader();
}

void Terrain::generateVertices()
{
	noise.SetNoiseType(noiseType);
	noise.SetFrequency(noiseFrequency);

	std::vector<std::vector<GLfloat>> colours = {
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};

	for (size_t row = 0; row < width; row++)
	{
		GLfloat rowOffset = row * tileSize;
		for (size_t col = 0; col < height; col++) {

			// position
			GLfloat y = noise.GetNoise(col, row);
			vertices.push_back((GLfloat)col * tileSize);
			vertices.push_back(y);
			vertices.push_back((GLfloat)rowOffset);

			// polygon lines colour
			std::vector<GLfloat> colour = colours[col % colours.size()];
			vertices.push_back(y * 2);
			vertices.push_back(y);
			vertices.push_back(colour[2]);

			// polygon fill colour
			vertices.push_back(fillR);
			vertices.push_back(fillG);
			vertices.push_back(fillB);
		}
	}
}

void Terrain::generateIndices()
{
	GLuint indexCounter = 0;
	for (size_t index = 0; (index + width) < (height * width); index += 1)
	{
		indices.push_back(index);
		indices.push_back(index + width);

		indexCounter += 1;
		if (indexCounter == width && (index + width) != (height * width) - 1) {
			indices.push_back(index + width);
			indices.push_back(index + 1);
			indexCounter = 0;
		}
	}
}

void Terrain::loadIntoShader() 
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// load pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, (void*)0);

	// center terrain
	model = glm::translate(model, glm::vec3(-((width / 2) * tileSize), -1.0f, -((height / 2) * tileSize)));

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.8f));

	// Adding all matrices up to create combined matrix
	mvp = projection * view * model;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Terrain::printConfig()
{
	std::cout << " -= Terrain Config =- " << std::endl;
	std::cout << "  Width: " << width << std::endl;
	std::cout << "  Height: " << height << std::endl;
	std::cout << "  tileSize: " << tileSize << std::endl;
	std::cout << "  noiseType: " << noiseType << std::endl;
	std::cout << "  noiseFrequency: " << noiseFrequency << std::endl;
}
