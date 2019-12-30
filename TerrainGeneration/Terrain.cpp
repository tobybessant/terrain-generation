#include "Terrain.h"

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency)
{
	width = _size;
	height = _size;
	tileSize = _tileSize;
	noiseType = _noiseType;
	noiseFrequency = _noiseFrequency;
	seed = rand();

	octaves = 4;
	magnitude = 4;

	createTerrain();
}

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency, GLuint _seed)
{
	width = _size;
	height = _size;
	tileSize = _tileSize;
	noiseType = _noiseType;
	noiseFrequency = _noiseFrequency;
	seed = _seed;

	octaves = 4;
	magnitude = 4;

	createTerrain();
}

void Terrain::increaseNoiseFrequency()
{
	noiseFrequency += (noiseFrequency * 0.1);
	updateHeightmap(false);
}

void Terrain::decreaseNoiseFrequency()
{
	noiseFrequency -= (noiseFrequency * 0.1);
	updateHeightmap(false);
}

void Terrain::increaseMagnitude()
{
	++magnitude;
	updateHeightmap(false);
}

void Terrain::decreaseMagnitude()
{
	--magnitude;
	updateHeightmap(false);
}

void Terrain::decreaseOctaves()
{
	if (octaves > 0) {
		--octaves;
		updateHeightmap(false);
	}
}

void Terrain::increaseOctaves()
{
	++octaves;
	updateHeightmap(false);
}

void Terrain::regenerateTerrain()
{
	updateHeightmap(true);
}

void Terrain::render(GLuint& program)
{
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
	generateVertices();
	generateIndices();
	loadIntoShader();
}

void Terrain::updateHeightmap(GLboolean useNewSeed)
{
	const int step = 9;

	std::vector<std::vector<GLfloat>> colours = {
		//  r     g     b
		{ 0.25f, 0.36f, 1.56f, }, // water
		{ 0.49f, 0.72f, 0.45f, }, // land
		{ 0.45f, 0.72f, 0.46f, }, // higher land
		{ 1.0f, 1.0f, 1.0f, } // snow
	};

	noise.SetNoiseType(noiseType);
	noise.SetFrequency(noiseFrequency);
	noise.SetFractalOctaves(octaves);

	if (useNewSeed) {
		noise.SetSeed(rand());
	}

	for (size_t row = 0; row < width; row++)
	{
		GLuint rowIndexOffset = row * width * step;
		for (size_t col = 0; col < height; col++) {
			GLuint colIndexOffset = col * step;
			GLuint vertexStartIndex = rowIndexOffset + colIndexOffset;
			GLfloat y = magnitude * noise.GetNoise(col, row);
			y = pow(y, 2);

			// y pos
			vertices[vertexStartIndex + 1] = y;

			// colour
			if (y < 0.06) {
				vertices[vertexStartIndex + 3] = colours[0][0];
				vertices[vertexStartIndex + 4] = colours[0][1];
				vertices[vertexStartIndex + 5] = colours[0][2];
			}												 
			else if (y < 1.2) {								 
				vertices[vertexStartIndex + 3] = colours[1][0];
				vertices[vertexStartIndex + 4] = colours[1][1];
				vertices[vertexStartIndex + 5] = colours[1][2];
			}												  
			else if (y < magnitude - (magnitude * 0.1)) {
				vertices[vertexStartIndex + 3] = colours[2][0];
				vertices[vertexStartIndex + 4] = colours[2][1];
				vertices[vertexStartIndex + 5] = colours[2][2];
			}												 
			else {											 
				vertices[vertexStartIndex + 3] = colours[3][0];
				vertices[vertexStartIndex + 4] = colours[3][1];
				vertices[vertexStartIndex + 5] = colours[3][2];
			}
		}
	}

	loadIntoShader();
}

glm::mat4 Terrain::getModel()
{
	return model;
}

void Terrain::generateVertices()
{
	noise.SetFractalOctaves(octaves);
	noise.SetNoiseType(noiseType);
	noise.SetFrequency(noiseFrequency);
	noise.SetSeed(seed);


	for (size_t row = 0; row < width; row++)
	{
		GLfloat rowOffset = row * tileSize;
		for (size_t col = 0; col < height; col++) {

			// position
			GLfloat y = magnitude * noise.GetNoise(col, row);
			y = pow(y, 2);

			vertices.push_back((GLfloat)col * tileSize);
			vertices.push_back(y);
			vertices.push_back((GLfloat)rowOffset);

			// polygon lines colour
			addColourForHeight(y);
			
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

void Terrain::addColourForHeight(GLfloat& y)
{
	std::vector<std::vector<GLfloat>> colours = {
		//  r     g     b
		{ 0.25f, 0.36f, 1.56f, }, // water
		{ 0.49f, 0.72f, 0.45f, }, // land
		{ 0.45f, 0.72f, 0.46f, }, // higher land
		{ 1.0f, 1.0f, 1.0f, } // snow
	};

	if (y < 0.06) {
		vertices.push_back(colours[0][0]);
		vertices.push_back(colours[0][1]);
		vertices.push_back(colours[0][2]);
	}
	else if (y < 1.2) {
		vertices.push_back(colours[1][0]);
		vertices.push_back(colours[1][1]);
		vertices.push_back(colours[1][2]);
	}
	else if (y < (magnitude - (magnitude * 0.1) )) {
		vertices.push_back(colours[2][0]);
		vertices.push_back(colours[2][1]);
		vertices.push_back(colours[2][2]);
	}
	else {
		vertices.push_back(colours[3][0]);
		vertices.push_back(colours[3][1]);
		vertices.push_back(colours[3][2]);
	}
}

void Terrain::loadIntoShader() 
{
	model = glm::mat4(1.0f);

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

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

