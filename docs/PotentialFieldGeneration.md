# Generating Potential Field

When the potential map is loaded, it will generate 2 main grids.
- Grid
    Grid will hold the value of the potential field based on distance from destination
- Shade
    Shade is created by a negative field generated from obstacles.

The grid will be updated every time when commands are issued. And when assigning potential field to agents. It will load the shade based on the agent type. Flying agents does not need to load the shade as they only affect ground agents.

This creates a 2 layer potential field for ground and air units. There can be an extra layer to mimic the behavior of Giant agents that ignores ground layer but still wall on ground. 
