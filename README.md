# PCG Noise-Based Terrain Generator
This project showcases the use of noise functions when procedurally generating 3D terrain.

## Generating Terrains / User Guide
This program runs from the command-line, and on starting will ask for various parameters to generate the base terrain. Once the parameters have been entered, it will open the generated terrain in fullscreen-windowed mode. The terrain can be live-updated and tweaked using various hot-keys specified below.

### Controls
| Key  | Action |
| ------------- | ------------- |
| W/A/S/D & Mouse | Fly / move camera |
| Q | Increase fly speed (toggle faster / slower) |
| Esc | Exit to terminal |
| R | Re-generate terrain with new seed |
| + | Increase noise frequency (10% increments)  |
| - | Decrease noise frequency (10% increments)  |
| Up Arrow | Increase terrain magnitude  |
| Down Arrow | Decrease terrain magnitude  |
| 0 | Increase octave count (fractal algorithms only)  |
| 9 | Decrease octave count (fractal algorithms only)  |
| i | 'Island-ify' terrain. Application will attempt to turn the generated terrain into an island surrounded by water.  |

### Terrain Parameters
Below are explanations of each of the terrain parameters and how they affect the generated terrain.

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
The application has 4 main tasks when creating the required user experience.
1. It has to gather initial terrain parameters from the user.
2. It has to build the terrain vertices, using noise to generate the height of each vertex.
3. It has to render the vertices.
4. It has to update & re-render the heightmap as the user tweaks the terrain.

### 1. Taking Terrain Parameters
To gather initial terrain parameters, I designed a ```ConsoleServices``` class that handles any console interaction. The ```ConsoleServices``` class has a number of methods to retrieve & validate user input, however the main function that returns the Terrain to be rendered is ```askForTerrain()```. This function gathers the parameters, builds a terrain, and returns it.

### 2. Build the Terrain Vertices
The largest part of the application, is the ```Terrain``` class. This class has a number of responsibilities, all related to setting, getting, generating and rendering the terrain. ```createTerrain()``` builds terrain vertices using the provided parameters. ```generateVertices()``` will iterate over the specified height / width of the grid and initialise vertex data for each point. This includes, position (with the height being set using the 'FastNoise' noise function), polygon line colour, and polygon fill colour.

### 3. Rendering the Terrain Vertices
The second responsibility of the Terrain class, is to render the terrain vertices. This is by generating the indices, loading the vertex & index data into the GPU buffers, rendering the fill colour, then rendering the polygon line colour. This two-step rendering process makes the terrain appear opaque, even when displaying the polygons.

### 4. Listen for Updates and Re-render
The final aspect of the application handles managing the terrain once it has been rendered. To allow the user to easily tweak the terrain without having to exit and re-enter all parameters, it was important to implement live-updating of terrain values. The ```InputManager``` class is used by GLFW to listen for user input, which it will then look up registered callbacks for in its ```keypressCallbacks``` dictionary. On program setup, the ```Terrain``` class registers any callbacks it needs. The actual updating is performed by editing a property on the terrain, and then updating the heightmap. This is done by iterating over the vertices, identifying which values represent the height of each vertex, and updating it accordingly. Once all vertices have been updated, the data is re-loaded into the GPU buffers.

### Other Implementation details
