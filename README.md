# Final project for CS 688

#### Compilation:
The project is written with `SFML2.4.2` and uses `premake4` to generate the makefile. `SFML2.4.2` is pre-compiled to a shared library with `g++5.0`.  

I've created a script to compile and run the program. Using the following commands to test the program.

The project only works under Linux with `premake4` installed and is only tested on `Ubuntu 16.04 LTS` (lab `gl10`).

```
$ ./configure
$ ./run
```

Note that the `configure` and `run` script need to have permission to be executed. I have tested on the lab computers and there is no permission problem but it is something to note. Also the `configure` file not not related to `autoconf` or something, it's just a shell script to run the build commands.

The program is able to take an input file as a map to load. If no argument is provided it will load the default `map.txt`. If a custom map is wished to load, you have to run the executable generated manually with the following command.

```
$ LD_LIBRARY_PATH=./SFML-2.4.2/lib ./rts [some map txt]
```

#### Manual
####### Controls
- `SPACE` toggles the view for displaying potential field
- Click and drag with left mouse to select agents. When agents are selected they will have visual changes. Such as a circle around it or change color.
- Scroll with zoom in/out of the view.
- Right click will issue "Commands" to current selected Agents
- Click and drag with middle mouse to move around the map.
####### Map format
- `grid width height width_blocks height_blocks` will load the grid of the map.
- `ab xpos ypos asset_path` will add a animation boid at `xpos ypos` on the map with `asset_path`.
- `b xpos ypos` will load a simple boid at `xpos ypos` on the map.
- `fb xpos ypos` will load a flying boid at `xpos ypos` on the map.
- `wf xpos ypos width height` will load a water fountain at `xpos ypos` with `width` and `height`.
- `st xpos ypos width height asset_path x_offset y_offset width_block height_block` will load a scene texture `asset_path` at `xpos ypos` with `width` and `height`. `x_offset` and `y_offset` defines the offset for it being a obstacle and `width_block` `height_block` defines how large are the obstacles.

#### Objectives
1. Interactive map with mini-map that will pan around and zoom base of the input of a mouse.
2. Generate a potential field base of a map and able to display it in a special mode.
3. Fog of war created base of agent location with alpha blending on the edges to create a soft finish.
4. Load a map from a local file to create a scene model for the map.
5. Apply textures to the loaded map base on scene model.
6. Particle system based water fountain as one of the scene objects
7. Multiply agents react to each other like boids.
8. Agents move based on physical models and correctly behave according to the potential field.
9. Able to select multiply agents and control their moving destination with the mouse.
10. More agents types that reacts to different layer of potential fields.

#### Extra Objectives
- Sprite animation agent types that updates the sprites based on current state.
