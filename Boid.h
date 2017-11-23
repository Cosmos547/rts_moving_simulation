#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>



class Boid {
    public:
        Boid(float xpos, float ypos);
        void update(float timestep);
        void calculate_forces(std::vector<Boid*> *boids, float dir_x, float dir_y);
        void render(sf::RenderWindow* window);
        float getDistance(Boid* b);
        float getOrientation();
        sf::Vector2f getPosition();
        sf::Vector2f limitVector(sf::Vector2f vec, float max);
        sf::Vector2f getSpeed();


    private:
        void calculate_speed(float timestep);
        void update_position(float timestep);
        float max_speed;
        float max_force;
        sf::Vector2f location;
        sf::Vector2f speed;
        sf::Vector2f force;
};
