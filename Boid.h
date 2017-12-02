#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "PotentialField.h"



class Boid {
    public:
        Boid(float xpos, float ypos);
        virtual void update(float timestep);
        void calculate_forces(std::vector<Boid*> *boids, sf::Vector2f dir);
        virtual void render(sf::RenderWindow* window);
        float getDistance(Boid* b);
        float getOrientation();
        sf::Vector2f getPosition();
        sf::Vector2f limitVector(sf::Vector2f vec, float max);
        sf::Vector2f getSpeed();
        void setDestination(sf::Vector2f desti);
        void setActive(bool b);
        bool getActiveState();
        void limitForceDir(sf::Vector2f dir);
        void setSelected(bool b);
        bool getSelected();
        void loadPF(PotentialField* p, int pid);
        int getPFID();
        int** getGrid();


    protected:
        void calculate_speed(float timestep);
        void update_position(float timestep);
        float max_speed;
        float max_force;
        float orientation;
        sf::Vector2f location;
        sf::Vector2f speed;
        sf::Vector2f force;
        sf::Vector2f pf;

        float size;

        PotentialField* field;
        int pfid;

        bool isActive;
        bool isSelected;

        sf::Vector2f desti;
};
