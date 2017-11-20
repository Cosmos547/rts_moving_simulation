#pragma once

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "SceneObject.h"


class PotentialMap {
    public:
        PotentialMap(float width, float height, int w, int h);
        ~PotentialMap();
        void update(float timestep);
        void render(sf::RenderWindow* window);


    private:
        float p_width;
        float p_height;
        int w_size;
        int h_size;
        float w_pixel;
        float h_pixel;
        std::vector<SceneObject*> sobjs;
        int** grid;
        void renderGrid(sf::RenderWindow* window);


};
