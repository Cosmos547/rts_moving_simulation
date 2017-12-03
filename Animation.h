#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>


class Animation {
    public:
        Animation(std::string assetName);
        void update(float elapsedTime, sf::Vector2f speed);
        void render(sf::RenderWindow* window, float xpos, float ypos, float angle, float size, sf::Vector2f speed);

    private:
        float time;
        int frame;
        sf::Texture* t;
        sf::Sprite* s;
        std::vector<sf::IntRect> txs;


};
