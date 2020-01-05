# PCG Noise-Based Terrain Generator
This project showcases the use of noise functions when procedurally generating 3D terrain. This project uses Auburns' [FastNoise](https://github.com/Auburns/FastNoise) library to generate the noise-based height data for a given vertex.

- [PCG Noise-Based Terrain Generator](#pcg-noise-based-terrain-generator)
  * [Generating Terrains / User Guide](#generating-terrains---user-guide)
    + [Setup](#setup)
      - [Executable](#executable)
      - [VS Project](#vs-project)
    + [Controls](#controls)
    + [Terrain Parameters](#terrain-parameters)
      - [CLI Parameters](#cli-parameters)
      - [Post-generation Parameters](#post-generation-parameters)
  * [Technical Design](#technical-design)
    + [1. Taking Terrain Parameters](#1-taking-terrain-parameters)
    + [2. Build the Terrain Vertices](#2-build-the-terrain-vertices)
    + [3. Rendering the Terrain Vertices](#3-rendering-the-terrain-vertices)
    + [4. Listen for Updates and Re-render](#4-listen-for-updates-and-re-render)
  * [Other Implementation Details](#other-implementation-details)
    + [Camera](#camera)
    + [Time](#time)
    + [FastNoise](#fastnoise)
- [Miscellaneous](#miscellaneous)
  * [Inspiration & Differences from Other Programs](#inspiration---differences-from-other-programs)
  * [What I Started With](#what-i-started-with)
  * [Video Report](#video-report)


## Generating Terrains / User Guide
This program runs from the command-line, and on starting will ask to either start the Terrain Wizard or provide a path to a .terrain file. The terrain wizard will ask for various parameters to generate the base terrain. Once the parameters have been entered/file loaded, it will open the generated terrain in fullscreen-windowed mode. The terrain can be live-updated and tweaked using the various hot-keys specified below.

### Setup
#### Executable
Unzip the 'Executable' folder and run TerrainGeneration.exe.

#### VS Project
Download/clone the solution. You will need to install the dependencies - they should already be in the NuGet Packages for the project, to install them:

1. Right Click Solution > 'Restore NuGet Packages for the solution'
2. Right Click Project > 'Rescan the project for dependencies'

At this point the project should be ready to build.

### Controls
| Key  | Action |
| ------------- | ------------- |
| W/A/S/D & Mouse | Fly / move camera. |
| C | Centre / reset camera position. |
| Q | Increase fly speed (toggle faster / slower). |
| Esc | Exit to terminal. |
| R | Re-generate terrain with current parameters & new seed. |
| + | Increase noise frequency (10% increments). |
| - | Decrease noise frequency (10% increments).  |
| Up Arrow | Increase terrain magnitude.  |
| Down Arrow | Decrease terrain magnitude.  |
| 0 | Increase octave count (fractal algorithms only).  |
| 9 | Decrease octave count (fractal algorithms only).  |
| i | 'Island-ify' terrain. The application will attempt to turn the existing terrain into an island surrounded by water. If the terrain has particularly high vertexes on the outer-edges, this button may need to be pressed more than once to completely separate the terrain from the edge. NOTE: tweaking any other properties of the terrain will undo the 'island-ification' and will need to be 're-island-ified'. |
| E | Export terrain config to .terrain file (file will be exported to 'exported_terrains' folder). |

### Terrain Parameters
Below are explanations of each of the terrain parameters and how they affect the generated terrain. The CLI Parameters are the parameters that the Terrain Wizard will collect from the user. The 'Post-generation Parameters' will be set to defaults when using the Terrain Wizard.

All parameters are set from the file contents when loading terrain data from file.

#### CLI Parameters
| Parameter  | Description |
| ------------- | ------------- |
| Terrain Size | The number of tiles that will make up the width and depth of the terrain. It is effectively the 'grid size'. For instance, entering 350 will result in a 350x350 grid of tiles. |
| Tile Size | The size of each tile in the terrain.  |
| Noise Type | The type of noise algorithm to be used. 'Perlin Fractal' is recommended for those new to using noise algorithms in terrain generation. |
| Noise Frequency | How 'coarse' the noise will be. The larger the terrain size, the smaller this should be. Any value larger than 0.1 will often result in non-terrain looking terrain as it is too granular. |
| Seed | Input an initial seed into the terrain, to start the noise generation from. Useful if you want to re-generate the same terrain. |

#### Post-generation Parameters
These are the parameters that can only be updated once the terrain has been generated.

| Parameter  | Initial Value | Description |
| :--------- | :-----: | :---------- |
| Noise Octaves | 4 | This parameter can only be edited for 'fractal' algorithms. Fractal algorithms use multiple layers (octaves) of noise at varying frequencies to 'break-up' the final noise generated. This parameter is the count of octaves to be used. This can add more texture to the terrain, making it look rougher, without directly increasing the noise frequency. |
| Terrain Magnitude | 4 |  This is a multiplier applied to each generated vertex height, to increase / decrease the height of terrain features.  |

## Technical Design
The application has 4 main tasks when creating the desired user experience.
1. Retrieve initial terrain parameters from the user (either from a .terrain file or using the terrain wizard).
2. Build the terrain vertices, using noise to generate the height of each vertex.
3. Render the terrain data.
4. Update & re-render the heightmap as the user tweaks the terrain.

### 1. Taking Terrain Parameters
To gather initial terrain parameters, I designed a ```ConsoleServices``` class that can handle console interactions. The ```ConsoleServices``` class has a number of methods to retrieve & validate user input. The first question the program asks is if the user would like to generate a terrain using the terrain wizard, or import data from a .terrain file. If the user choses to use the Terrain Wizard, the ```askForTerrain()``` method collects parameters from the user and returns the Terrain to be rendered. If the user choses to load parameters from a .terrain file, the console asks for a path, and then uses ```loadTerrainFromFile()``` to read & return a new terrain from the file's contents.

### 2. Build the Terrain Vertices
The largest part of the application is the ```Terrain``` class. This class has a number of responsibilities, all related to getting, setting, generating and rendering the terrain. ```createTerrain()``` builds terrain vertices using the provided parameters. ```generateVertices()``` will iterate over the specified height / width of the grid and initialise vertex data for each point. This includes position (with the height being set using the 'FastNoise' noise function), polygon line colour, and polygon fill colour.

### 3. Rendering the Terrain Vertices
The second responsibility of the Terrain class, is to render the terrain vertices. This is done by generating the indices, loading the vertex & index data into the GPU buffers, rendering the fill colour, then rendering the polygon line colour. This two-step rendering process makes the terrain appear opaque, even when displaying the polygon triangles.

### 4. Listen for Updates and Re-render
The final aspect of the application handles managing the terrain once it has been rendered. This allows the user to easily tweak the terrain without having to exit and re-enter all of parameters. The ```InputManager``` class is used by GLFW to listen for user input - on keypress, the ```InputManager``` class will look up registered callbacks for the pressed key in its ```keypressCallbacks``` dictionary. On program setup, the various ```Terrain``` class update methods are registered to their corresponding keys. The actual update process consists of editing a property on the terrain, and then updating the vertex data. Updating the heightmap is done by iterating over the vertices, identifying which values represent the height of each vertex, and regenerating it accordingly. Once all vertices have been updated, the data is re-loaded into the GPU buffers for rendering.

## Other Implementation Details
### Camera
To enable the player to 'fly' around the terrain they have created, I designed a ```Camera``` class, that uses the ```Input Manager``` to both register key presses, as well as subscribe to any new mouse updates through the use of an observer pattern.

### Time
To ensure particular updates are consistent across machines - such as the speed of the camera - I added a ```Time``` class that tracks the current frame state and calculates the ```deltaTime``` which can then be used in the camera movement calculations.

### FastNoise
Auburn's FastNoise library has been used to generate the noise-based height map. It was chosen based on its optimal combination of noise-generation speed and feature set. With 9 different noise types, it allows my program to offer many different outputs to the user. The library can generate noise based on provided X & Y coordinates. This worked well with my set up, as I generate the grid (columns and rows) at the same time as I build their vertex data, allowing me to generate the height of each vertex with their current column (x) and row (y) values.

# Miscellaneous
## Inspiration & Differences from Other Programs
The main driver for creating the application was to learn something about PCG, as it is a topic I am interested in but never had the opportunity to implement it in software. The decision to use noise-based terrain generation was based on it being an achievable PCG implementation for the given time frame.

Noise-based terrain generation is very popular, however I have made an effort to make my program unique where possible. The main features that differentiate my program from other programs are the ability to 'fly' through the terrain, this allows the user to see their terrain from many different angles / perspectives. Secondly, I believe my 'island-ification' functionality is fairly unique, having defined my own curve function to lower vertex heights depending on their distance from the centre.

## What I Started With
This project started from a new/blank project. The only resource pulled across / not originally written for this submission was the module-provided sample ShaderLoader code, to load my simplified shaders into a shader program.

## Video Report
The video for this report can be accessed at: https://www.youtube.com/watch?v=qH41VISRSoA
