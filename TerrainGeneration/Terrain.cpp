#include "Terrain.h"

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency) :
	width(_size), height(_size), tileSize(_tileSize), noiseType(_noiseType), noiseFrequency(_noiseFrequency), seed(rand())
{
	setDefaults();
	calculateMaxDistance();
	createTerrain();
}

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency, GLint _seed) :
	width(_size), height(_size), tileSize(_tileSize), noiseType(_noiseType), noiseFrequency(_noiseFrequency), seed(_seed)
{
	setDefaults();
	calculateMaxDistance();
	createTerrain();
}

Terrain::Terrain(GLuint _size, GLfloat _tileSize, FastNoise::NoiseType _noiseType, GLfloat _noiseFrequency, GLint _seed, GLint _noiseOctaves, GLint _noiseMagnitude, GLboolean _isIsland) :
	width(_size), height(_size), tileSize(_tileSize), noiseType(_noiseType), noiseFrequency(_noiseFrequency), seed(_seed), octaves(_noiseOctaves), magnitude(_noiseMagnitude)
{
	createTerrain();
	calculateMaxDistance();
	if (_isIsland) {
		makeIsland();
	}
}

std::string Terrain::getTerrainConfigString()
{
	std::string result = "";

	result += "terrain_size=" + std::to_string(width) + "\n";
	result += "tile_size=" + std::to_string(tileSize) + "\n";
	result += "noise_type=" + std::to_string(noise.GetNoiseType()) + "\n";
	result += "noise_seed=" + std::to_string(noise.GetSeed()) + "\n";
	result += "noise_frequency=" + std::to_string(noise.GetFrequency()) + "\n";
	result += "noise_octaves=" + std::to_string(noise.GetFractalOctaves()) + "\n";
	result += "noise_magnitude=" + std::to_string(magnitude) + "\n";
	result += "is_island=" + std::to_string(isIsland) + "\n";

	return result;
}

glm::vec3 Terrain::getFirstVertexPosition()
{
	// returns position of first vertex in grid , before the terrain is centre translated (so this will return { 0, noise(), 0 })
	return glm::vec3(vertices[0], vertices[1], vertices[2]);
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

void Terrain::makeIsland()
{
	// step for each vertex data set
	const int step = 9;

	// iterate rows and columns, and insert new height & colour values 
	for (GLint row = 0; row < width; row++)
	{
		// find the current index offset for the row
		GLuint rowIndexOffset = row * width * step;
		for (GLint col = 0; col < height; col++) {
			// find the current index offset for the column
			GLuint colIndexOffset = col * step;

			// find the index for the vertex data
			GLuint vertexStartIndex = rowIndexOffset + colIndexOffset;

			// find X and Y distance from the center
			GLfloat diffX = fabsf(centerX - col);
			GLfloat diffY = fabsf(centerY - row);

			// calculate normalised distance
			GLfloat distance = sqrtf(pow(diffX, 2) + pow(diffY, 2));
			GLfloat distanceNrm = distance / maxDistance;

			// fetch current value
			GLfloat y = vertices[vertexStartIndex + 1];
			
			// modify value using its distance, based on exponential curve
			y = y * (1 - distanceNrm) + pow(M_E, (-5 * distanceNrm)) - distanceNrm;

			// if y is now less than 0, return it to 0
			if (y < 0.0f)
				y = 0.0f;

			// set index to new value
			vertices[vertexStartIndex + 1] = y;

			// update colour
			updateColourForHeight(vertexStartIndex, y);
		}
	}

	loadIntoShader();
	isIsland = true;
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

void Terrain::setDefaults()
{
	octaves = 4;
	magnitude = 4;
}

void Terrain::calculateMaxDistance()
{
	// calculate the center vertex indexes
	centerX = roundf(width / 2);
	centerY = roundf(height / 2);

	// calculate the largest possible distance so distance calculations can be normalised
	maxDistance = sqrtf(pow(fabsf(centerX), 2) + pow(fabsf(centerY), 2));
}

void Terrain::createTerrain()
{
	generateVertices();
	generateIndices();
	initBuffers();
	loadIntoShader();
}

void Terrain::updateHeightmap(GLboolean useNewSeed)
{
	// step for each vertex data set
	const int step = 9;

	// update noise values
	noise.SetNoiseType(noiseType);
	noise.SetFrequency(noiseFrequency);
	noise.SetFractalOctaves(octaves);

	if (useNewSeed) {
		noise.SetSeed(rand());
	}

	// iterate rows and columns, and insert new height & colour values 
	for (GLint row = 0; row < width; row++)
	{
		// find the current index offset for the row
		GLuint rowIndexOffset = row * width * step;
		for (GLint col = 0; col < height; col++) {
			// find the current index offset for the column
			GLuint colIndexOffset = col * step;

			// find the index for the vertex data
			GLuint vertexStartIndex = rowIndexOffset + colIndexOffset;

			// calculate new height value
			GLfloat y = noise.GetNoise(col, row) * magnitude;
			y = pow(y, exponent);

			// update y value
			vertices[vertexStartIndex + 1] = y;

			// update colour for new height
			updateColourForHeight(vertexStartIndex, y);
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
	// set FastNoise noise properties to current terrain config
	noise.SetFractalOctaves(octaves);
	noise.SetNoiseType(noiseType);
	noise.SetFrequency(noiseFrequency);
	noise.SetSeed(seed);

	// iterate w/h of the terrain and add vertex data for each position
	for (GLint row = 0; row < width; row++)
	{
		// calculate the current spatial offset for the next row based on tilSize
		GLfloat rowOffset = row * tileSize;
		for (GLint col = 0; col < height; col++) {

			// position data
			// calculate height data - use FastNoise and then apply magnitude / exponent modifications
			GLfloat y = magnitude * noise.GetNoise(col, row);
			y = pow(y, exponent);

			// add positional data to vertex array
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
	// initialise index counter to track current index count for degenerate triangles
	GLuint indexCounter = 0;

	// iterate whilst there are stll indexes left to add
	for (size_t index = 0; (index + width) < (height * width); index += 1)
	{
		// add indexes for current position, and the index of the vertex directly below
		indices.push_back(index);
		indices.push_back(index + width);

		// add 1 to index counter for this row
		indexCounter += 1;

		// if we are at the end of the row, we need to add a 'degenerate' triangle (triangle 
		// with no area - automatically removed by OpenGL). This prevents triangles being drawn
		// that connect the end of one row with the start of the next.
		if (indexCounter == width && (index + width) != (height * width) - 1) {
			// add two indexes - one for the last vertex of this row, and one for the 
			// first vertex of the next row.
			indices.push_back(index + width);
			indices.push_back(index + 1);

			// reset counter for next row
			indexCounter = 0;
		}
	}
}

void Terrain::initBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Terrain::updateColourForHeight(GLuint& startIndex, GLfloat& y)
{
	if (y < 0.06) {
		vertices[startIndex + 3] = colours[0][0];
		vertices[startIndex + 4] = colours[0][1];
		vertices[startIndex + 5] = colours[0][2];
	}
	else if (y < 1.2) {
		vertices[startIndex + 3] = colours[1][0];
		vertices[startIndex + 4] = colours[1][1];
		vertices[startIndex + 5] = colours[1][2];
	}
	else if (y < magnitude - (magnitude * 0.1)) {
		vertices[startIndex + 3] = colours[2][0];
		vertices[startIndex + 4] = colours[2][1];
		vertices[startIndex + 5] = colours[2][2];
	}
	else {
		vertices[startIndex + 3] = colours[3][0];
		vertices[startIndex + 4] = colours[3][1];
		vertices[startIndex + 5] = colours[3][2];
	}
}

void Terrain::addColourForHeight(GLfloat& y)
{
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

	// Reset island flag, will be set to true after this function only in the 'makeIsland()' method. 
	// Other update methods will not set this flag to true
	isIsland = false;
}

