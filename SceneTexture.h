#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneObject.h"


class SceneTexture : SceneObject {
    public:
        SceneTexture(float xpos, float ypos, float xsize, float ysize, std::string textureName);
        virtual void render(sf::RenderWindow* window);

    private:
        sf::Texture t;


};
