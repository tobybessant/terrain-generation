#include "ConsoleServices.h"

Terrain* ConsoleServices::askForTerrain()
{
	clearConsole();
	cout << "  -=| NEW TERRAIN |=-\n\n" << endl;

	// ask for terrain size
	GLuint terrainSize;
	askForUnsignedInt("-= Terrain Size =-",
					  "Single integer value that will be the height & width of the terrain. RECOMMENDED: 500 - 1500",
					  &terrainSize);
	
	clearConsole();
	printTerrainConfig(terrainSize);

	// ask for tile size
	GLfloat tileSize;
	askForFloat("-= Tile Size =-",
				"Width and height of each tile. RECOMMENDED: 0.01 - 0.2",
				&tileSize);

	clearConsole();
	printTerrainConfig(terrainSize, tileSize);

	// ask for noise type
	GLuint selectedNoiseTypeIndex;
	FastNoise::NoiseType noiseType;
	vector<string> noiseTypes { "Value", "Value Fractal", "Perlin", "Perlin Fractal", "Simplex", "Simplex Fractal", "Cellular", "White Noise", "Cubic", "Cubic Fractal" };
	string noiseTypesDescription = "Noise algorithm used to generate the height map.\n";

	for (int i = 0; i < noiseTypes.size(); i++)
	{
		noiseTypesDescription += to_string(i) + " = " + noiseTypes[i] + "\n";
	}
	noiseTypesDescription += "\n  Enter desired noise index: ";

	askForUnsignedInt("-= Noise Type =-", noiseTypesDescription, &selectedNoiseTypeIndex, noiseTypes.size() - 1);
	noiseType = static_cast<FastNoise::NoiseType>(selectedNoiseTypeIndex);


	clearConsole();
	printTerrainConfig(terrainSize, tileSize, noiseTypes[selectedNoiseTypeIndex]);

	GLfloat noiseFrequency;
	askForFloat("-= Noise Frequency =-",
				"Frequency of the noise to be generated. RECOMMENDED: 0.01 - 0.1",
				&noiseFrequency);

	clearConsole();
	printTerrainConfig(terrainSize, tileSize, noiseTypes[selectedNoiseTypeIndex], noiseFrequency);

	return new Terrain(terrainSize, tileSize, noiseType, noiseFrequency);
}

void ConsoleServices::clearConsole()
{
	for (size_t i = 0; i < 30; i++)
	{
		cout <<endl;
	}
}

void ConsoleServices::printLine(const vector<string>& segments)
{
	cout << "  ";
	for (size_t i = 0; i < segments.size(); i++)
	{
		cout << segments[i] << " ";
	}
	cout << endl;
}

void ConsoleServices::askForUnsignedInt(string name, string description, GLuint* response)
{
	string userResponse = "";
	do {
		if (!userResponse.empty())
			printLine({ "Invalid value. Please enter a number greater than 0." });

		printLine({ name });
		printLine({ description });

		cout << "  ->  ";
		cin >> userResponse;
	} while (!isNumber(userResponse) || stoi(userResponse) <= 0);

	*response = stoi(userResponse);
}

void ConsoleServices::askForUnsignedInt(string name, string description, GLuint* response, GLuint max)
{
	string userResponse = "";
	do {
		if (!userResponse.empty())
			printLine({ "Invalid value. Please enter a number greater than 0, but less than", to_string(max) });

		printLine({ name });
		printLine({ description });

		cout << "  ->  ";
		cin >> userResponse;
	} while (!isNumber(userResponse) || stoi(userResponse) <= 0 && stoi(userResponse) > max);

	*response = stoi(userResponse);
}

void ConsoleServices::askForFloat(string name, string description, GLfloat* response)
{
	string userResponse = "";
	do {
		printLine({ name });
		printLine({ description });

		cout << "  ->  ";
		cin >> userResponse;
	} while (!isFloat(userResponse));

	*response = stof(userResponse);
	
}

bool ConsoleServices::isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

bool ConsoleServices::isFloat(const std::string& s)
{
	GLint decimalCount = 0;
	std::string::const_iterator it = s.begin();
	while (it != s.end() && (std::isdigit(*it) || *it == '.')) {
		if (*it == '.') decimalCount++;
		++it;
	}
	return !s.empty() && it == s.end() && decimalCount >= 1;
}

void ConsoleServices::printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType, GLfloat& noiseFrequency)
{
	printLine({ "+ -= Terrain Config =- " });
	printLine({ "| Width: ", to_string(terrainSize) });
	printLine({ "| Height: ", to_string(terrainSize) });
	printLine({ "| Tile Size: ", to_string(tileSize) });
	printLine({ "| Noise Type: ", slectedNoiseType });
	printLine({ "| Noise Frequency: ", to_string(noiseFrequency) });
	printLine({ "+ -------------------" });

	printLine({ "| Building terrain. . ." });
	printLine({ "+ ------------------- " });
	printLine({ "\n\n" });
}

void ConsoleServices::printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType)
{
	printLine({ "+ -= Terrain Config =- " });
	printLine({ "| Width: ", to_string(terrainSize) });
	printLine({ "| Height: ", to_string(terrainSize) });
	printLine({ "| Tile Size: ", to_string(tileSize) });
	printLine({ "| Noise Type: ", slectedNoiseType });
	printLine({ "+ -------------------" });
	
	printLine({ "\n\n" });
}

void ConsoleServices::printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize)
{
	printLine({ "+ -= Terrain Config =- " });
	printLine({ "| Width: ", to_string(terrainSize) });
	printLine({ "| Height: ", to_string(terrainSize) });
	printLine({ "| Tile Size: ", to_string(tileSize) });
	printLine({ "+ -------------------" });

	printLine({ "\n\n" });
}

void ConsoleServices::printTerrainConfig(GLuint& terrainSize)
{
	printLine({ "+ -= Terrain Config =- " });
	printLine({ "| Width: ", to_string(terrainSize) });
	printLine({ "| Height: ", to_string(terrainSize) });
	printLine({ "+ -------------------" });

	printLine({ "\n\n" });
}
