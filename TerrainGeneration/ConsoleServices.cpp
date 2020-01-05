#include "ConsoleServices.h"

Terrain* ConsoleServices::askForTerrain()
{
	clearConsole();
	string response;
	do {
		printLine({ "How would you like to generate a terrain?" });
		printLine({ "0 = CLI Terrain Wizard" });
		printLine({ "1 = Load from .terrain file" });
		cout << "  ->";
		cin >> response;

		if (response == "0") {
			return startTerrainWizard();
		}
		else if (response == "1") {
			string path;
			askForTerrainPath(&path);
			if (path != "BACK") {
				return loadTerrainFromFile(path);
			}
		}
		clearConsole();
	} while (true);
}

Terrain* ConsoleServices::startTerrainWizard()
{
	clearConsole();
	printLine({"-= NEW TERRAIN =-\n"});

	// ask for terrain size
	GLuint terrainSize;
	askForUnsignedInt("-= Terrain Size =-",
					  "Provide one whole number value that will be the height & width of the terrain. RECOMMENDED: 300 - 600",
					  &terrainSize);
	clearConsole();
	printTerrainConfig(terrainSize);

	// ask for tile size
	GLfloat tileSize;
	askForFloat("-= Tile Size =-",
				"Width and height of each tile. RECOMMENDED: 0.05 - 0.3",
				&tileSize);
	clearConsole();
	printTerrainConfig(terrainSize, tileSize);

	// ask for noise type
	GLuint selectedNoiseTypeIndex;
	FastNoise::NoiseType noiseType;
	vector<string> noiseTypes { "Value", "Value Fractal", "Perlin", "Perlin Fractal", "Simplex", "Simplex Fractal", "Cellular", "White Noise", "Cubic", "Cubic Fractal" };
	string noiseTypesDescription = "Noise algorithm used to generate the height map. Fractal algorithms will have access to altering the number of Octaves (noise layers) used.\n";

	for (int i = 0; i < noiseTypes.size(); i++)
	{
		noiseTypesDescription += "  " + to_string(i) + " = " + noiseTypes[i] + "\n";
	}

	noiseTypesDescription += "\n  Enter desired noise index: ";
	askForUnsignedInt("-= Noise Type =-", noiseTypesDescription, &selectedNoiseTypeIndex, noiseTypes.size() - 1);
	noiseType = static_cast<FastNoise::NoiseType>(selectedNoiseTypeIndex);
	clearConsole();
	printTerrainConfig(terrainSize, tileSize, noiseTypes[selectedNoiseTypeIndex]);

	// ask for noise frequency
	GLfloat noiseFrequency;
	askForFloat("-= Noise Frequency =-",
				"Frequency of the noise to be generated. RECOMMENDED: 0.01",
				&noiseFrequency);
	clearConsole();
	printTerrainConfig(terrainSize, tileSize, noiseTypes[selectedNoiseTypeIndex], noiseFrequency);

	// ask for noise seed
	string seed;
	string seedDisplay = "Random";
	askForSeed("-= Noise Seed =-", 
			   "Enter a terrain seed (any positive or negative whole integer (max 10 digits), for random seed enter 'r')", 
			   &seed);
	clearConsole();
	
	Terrain* t = nullptr;
	if (isNumber(seed)) {
		seedDisplay = seed;
		printTerrainConfig(terrainSize, tileSize, noiseTypes[selectedNoiseTypeIndex], noiseFrequency, seedDisplay);
		
		t = new Terrain(terrainSize, tileSize, noiseType, noiseFrequency, stoi(seed));
	}
	else {
		printTerrainConfig(terrainSize, tileSize, noiseTypes[selectedNoiseTypeIndex], noiseFrequency, seedDisplay);
		t = new Terrain(terrainSize, tileSize, noiseType, noiseFrequency);
	}
	
	return t;
}

Terrain* ConsoleServices::loadTerrainFromFile(string& path)
{
	FILE* fp;
	char line[100];

	vector<string> obtainedVariables = {};
	const vector<string> requiredVariables = { "terrain_size", "tile_size", "noise_type", "noise_frequency", "seed", "noise_octaves", "noise_magnitude", "is_island" };

	GLuint size;
	GLfloat tileSize;
	FastNoise::NoiseType noiseType;
	GLfloat noiseFrequency;
	GLint seed;
	GLint noiseOctaves;
	GLint noiseMagnitude;
	GLboolean isIsland;

	errno_t err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			// size
			if (strstr(line, requiredVariables[0].c_str())) {
				size = stoi(getValue(line));
				obtainedVariables.push_back(requiredVariables[0]);
			}
			// tile_size
			if (strstr(line, requiredVariables[1].c_str())) {
				tileSize = stof(getValue(line));
				obtainedVariables.push_back(requiredVariables[1]);
			}
			// noise_type
			if (strstr(line, requiredVariables[2].c_str())) {
				noiseType = FastNoise::NoiseType(stoi(getValue(line)));
				obtainedVariables.push_back(requiredVariables[2]);
			}
			// noise_frequency
			if (strstr(line, requiredVariables[3].c_str())) {
				noiseFrequency = stof(getValue(line));
				obtainedVariables.push_back(requiredVariables[3]);
			}
			// seed
			if (strstr(line, requiredVariables[4].c_str())) {
				seed = stoi(getValue(line));
				obtainedVariables.push_back(requiredVariables[4]);
			}
			// noise_octaves
			if (strstr(line, requiredVariables[5].c_str())) {
				noiseOctaves = stoi(getValue(line));
				obtainedVariables.push_back(requiredVariables[5]);
			}
			// noise_magnitude
			if (strstr(line, requiredVariables[6].c_str())) {
				noiseMagnitude = stoi(getValue(line));
				obtainedVariables.push_back(requiredVariables[6]);
			}
			// is_island
			if (strstr(line, requiredVariables[7].c_str())) {
				isIsland = stoi(getValue(line));
				obtainedVariables.push_back(requiredVariables[7]);
			}
		}
	}
	else 
	{
		cout << "Error opening file " + path << endl;
		return nullptr;
	}

	if (obtainedVariables.size() == requiredVariables.size()) {
		return new Terrain(size, tileSize, noiseType, noiseFrequency, seed, noiseOctaves, noiseMagnitude, isIsland);
	}
	else 
	{
		cout << "Malformed terrain file. MISSING: [ ";
		for (string variable : requiredVariables) {
			if (std::count(obtainedVariables.begin(), obtainedVariables.end(), variable) == 0)
			{
				cout << variable + ", ";
			}
		}
		cout << " ] " << endl;
		return nullptr;
	}
}

void ConsoleServices::askForTerrainPath(string* response)
{
	string userResponse = "";
	do {
		if (!userResponse.empty())
			printLine({ "Invalid value. Please enter a valid terrain path." });

		printLine({ "Please enter the path to a .terrain file. Enter 'BACK' to return home." });

		cout << "  ->  ";
		cin >> userResponse;
	} while (!userResponse.compare(".terrain") == 0 && !isFile(userResponse) && userResponse != "BACK");

	*response = userResponse;
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

void ConsoleServices::askForSeed(string name, string description, string* response)
{
	string userResponse = "";
	do {
		if (!userResponse.empty())
			printLine({ "Invalid value. Please enter one positive/negative number no longer than 9 digits in length" });

		printLine({ name });
		printLine({ description });

		cout << "  ->  ";
		cin >> userResponse;
	} while (userResponse.size() > 9);

	*response = userResponse;
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

bool ConsoleServices::isFile(std::string& path)
{
	FILE* fp;
	errno_t err;

	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		return true;
	}
	return false;
}

void ConsoleServices::printTerrainConfig(GLuint& terrainSize, GLfloat& tileSize, string& slectedNoiseType, GLfloat& noiseFrequency, string& seed)
{
	printLine({ "+ -= Terrain Config =- " });
	printLine({ "| Width: ", to_string(terrainSize) });
	printLine({ "| Height: ", to_string(terrainSize) });
	printLine({ "| Tile Size: ", to_string(tileSize) });
	printLine({ "| Noise Type: ", slectedNoiseType });
	printLine({ "| Noise Frequency: ", to_string(noiseFrequency) });
	printLine({ "| Noise Seed: ", seed });
	printLine({ "+ -------------------" });

	printLine({ "| Building terrain. . ." });
	printLine({ "+ ------------------- " });
	printLine({ "\n\n" });
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

std::string ConsoleServices::getValue(char* line)
{
	// remove newline character
	char* pos;
	if ((pos = strchr(line, '\n')) != NULL)
		*pos = '\0';

	// duplicate as to not lose the variable when scope is left
	string lineString = (string)line;
	string duplicate = lineString;

	char* token;
	char* nextToken = nullptr;

	// array to store final value
	char* values[1]{ 0 };
	int counter = 0;

	// tokenise on space and iterate over line
	token = strtok_s((char*)duplicate.c_str(), "=", &nextToken);
	while (token != NULL) {
		// if element is not the first, add to values array
		if (counter > 0) {
			values[counter - 1] = token;
		}
		// next token
		counter++;
		token = strtok_s(NULL, "=", &nextToken);
	}

	// return string-casted value
	return (string)values[0];
}

void ConsoleServices::exportToFile(std::string& data)
{
	std::string time = to_string(std::time(NULL));
	fstream file;

	file.open("terrain_exports/export_" + time + ".terrain", ios::out);
	if (file)
	{
		file << data << endl;
		file.close();

		cout << "File 'export_" + time + ".terrain' created successfully." << endl;
	}
	else 
	{
		cout << "Error in creating file. Please ensure folder 'terrain_exports' exists in root folder." << endl;
	}
}


