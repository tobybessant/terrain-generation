# PCG Noise-Based Terrain Generator
This project aims to showcase the use of noise functions when procedurally generating 3D terrain.

## Generating Terrains / User Guide
This program runs from the command-line, and upon opening will ask for various parameters to generate the base terrain. Once the parameters have been entered, it will open the generated terrain in fullscreen-windowed mode. The terrain can be live-updated.

### Controls
| Key  | Action |
| ------------- | ------------- |
| W/A/S/D & Mouse | Fly / move camera |
| Q | Increase fly speed (toggle faster / slower) |
| Esc | Exit to terminal |
| R | Live re-generate terrain with current parameters |
| + | Increase noise frequency (10% increments)  |
| - | Decrease noise frequency (10% increments)  |
| Up Arrow | Increase terrain magnitude  |
| Down Arrow | Decrease terrain magnitude  |
| 0 | Increase octave count (fractal algorithms only)  |
| 9 | Decrease octave count (fractal algorithms only)  |

### Terrain Parameters
Below are explanations of each of the terrain parameters and how they affect the generated terrain.

#### CLI Parameters
| Parameter  | Description |
| ------------- | ------------- |
| Terrain Size | This value is the number of tiles that will make up the width and depth of the terrain. It is effectively the 'grid size'. For instance, entering 350 as this parameter will result in a 350x350 grid of tiles. |
| Tile Size | The size of each tile in the terrain.  |
| Noise Type | The type of noise algorithm to be used. 'Perlin Fractal' is recommended for first time users. |
| Noise Frequency | How 'coarse' the noise will be, the larger the terrain size, the smaller this should be. Any value larger than 0.1 will result in non-terrain looking terrain as it is too granular. |
| Seed | Input an initial seed into the terrain, to start the noise generation from. Useful if you want to re-generate the same terrain. |

#### Post-generation Parameters
These are the parameters that can only be updated once the terrain has been generated.

| Parameter  | Initial Value | Description |
| :--------- | :-----: | :---------- |
| Noise Octaves | 4 | This parameter can only be edited for 'fractal' algorithms. Fractal algorithms use multiple layers of noise at varying frequencies to 'break-up' the resulting noise. This adds more texture to the terrain, making it look rougher, without directly increasing the noise frequency. |
| Terrain Magnitude | 4 |  This is a multiplier applied to each generated vertex height, to increase / decrease the height of terrain features.  |
