#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>

enum class Dir {UL, U, UR, L, S, R, DL, D, DR};

class Animation {
    public:
        Animation();
        void update(float elapsedTime);
        void render(sf::RenderWindow* window, Dir dir);

    private:
        float time;
        int frame;
        sf::Texture* t;
        sf::Sprite* s;

};
