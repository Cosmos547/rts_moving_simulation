#Boids

I implemented the agents as real boids at first. However, whey will still forever to move to the final destination and they will never stop. So I tweaked the equation a bit and here are some results of different configurations.

In my final implementation, every force is normalized to some extent. So that, by changing the coefficients of each force we can have different behaviors. The most hard thing I found is to tweak the ration of potential field force.


When the potential field force is too large, when the agents are moving they tend to collide with each other. My first thought is to increase the separation force. However, after achieving a satisfiable separation between the agents. They will move like bumping to each other and collide away very fast. And when decreasing the potential field force lower to a point, cohesion and alignment forces will overtake the movement of the agents and they will not move towards the destination.

After tweaking the parameters so much, currently the simulation looks decent to my opinion. However, at a later state why I want to implement different types of agents and calculate the distance based on their real distance not center to center distance. Some problem occurs with the current model. Since we are looking at real distance, both the neighbor distance and separation distance needs to be tweaked. When I tried this model, I realized that a k-d tree will not work since the definition of distance is different. After spend many time tweaking this version, I could not get anything that feels very good. So I stuck with center to center distance and worked on other aspect of the simulation as I am wasting too much time on it.
