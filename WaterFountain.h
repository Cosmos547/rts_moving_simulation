#pragma once
#include "SceneObject.h"
#include <SFML/Graphics.hpp>
#include <stdlib.h>


class WaterFountain : public SceneObject{
    public:
        WaterFountain(float xpos, float ypos, float xsize, float ysize);
        void render(sf::RenderWindow* window);
        void update(float timestep);


    private:
        float xpos;
        float ypos;
        float xsize;
        float ysize;
        float x_cen;
        float y_cen;
        float radius;
        float pt_speed;

        float generationRate;

        sf::CircleShape cir;
        
        float gravity;
        
        // A particle is a vector of float that contains the states of the particle
        // It's in the following order: X-pos, Y-pos, angle, lifespan
        std::vector<float*> particles;

        float* generateRandomParticle();
        void updateParticle(float* pt, float time);
        void removeDeprecateParticle();

};
