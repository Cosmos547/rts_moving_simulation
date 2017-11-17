#pragma once

#include <SFML/Graphics.hpp>

class SceneObject {
    public:
        SceneObject(float xpos, float ypos, float xsize, float ysize);
        virtual void update(float timestep);
        virtual void render(sf::RenderWindow* window);


    private:
        float xpos;
        float ypos;
        float xsize;
        float ysize;

};
