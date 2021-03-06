#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "Animation.h"

Animation::Animation(std::string assetName) {
    time = 0.0f;
    int frame = 0;
    t = new sf::Texture();
    t->loadFromFile(assetName);
    s = new sf::Sprite();

    s->setTexture(*t);
    s->setPosition({100, 100});
    txs.push_back(sf::IntRect(0,0,100,100));
    txs.push_back(sf::IntRect(100,0,100,100));
    txs.push_back(sf::IntRect(200,0,100,100));
    txs.push_back(sf::IntRect(300,0,100,100));
    txs.push_back(sf::IntRect(0,100,100,100));
    txs.push_back(sf::IntRect(100,100,100,100));
    txs.push_back(sf::IntRect(200,100,100,100));
    txs.push_back(sf::IntRect(300,100,100,100));
    txs.push_back(sf::IntRect(0,200,100,100));
    txs.push_back(sf::IntRect(100,200,100,100));
    txs.push_back(sf::IntRect(200,200,100,100));
    txs.push_back(sf::IntRect(300,200,100,100));
    txs.push_back(sf::IntRect(100,0,-100,100));
    txs.push_back(sf::IntRect(200,0,-100,100));
    txs.push_back(sf::IntRect(300,0,-100,100));
    txs.push_back(sf::IntRect(400,0,-100,100));
}


void Animation::update(float elapsedTime, sf::Vector2f speed) {
    float vspeed = sqrt(speed.x * speed.x + speed.y * speed.y);
    time += elapsedTime * vspeed;
    while (time > 10) {
        time -= 10;
    }
    if (time < 2.5) {
        frame = 0;
    }
    else if (time < 5) {
        frame = 1;
    }
    else if (time < 7.5) {
        frame = 2;
    }
    else {
        frame = 3;
    }
}


void Animation::render(sf::RenderWindow* window, float xpos, float ypos, float angle, float size, sf::Vector2f speed){
    if (abs(speed.x) + abs(speed.y) < 0.001) {
        frame = 0;
        time = 0;
    }
    float scale = size / s->getLocalBounds().width;
    int ess = 0;
    if (angle >= 45 && angle < 135) {
        ess = 3;
    } else if (angle >= 135 && angle < 225) {
        ess = 2;
    } else if (angle >= 225 && angle < 315) {
        ess = 1;
    }

    if (ess == 0) {
        s->setTextureRect(txs[12+frame]);
    }
    else if (ess == 1) {
        s->setTextureRect(txs[4+frame]);
    }
    else if (ess == 2) {
        s->setTextureRect(txs[frame]);
    }
    else {
        s->setTextureRect(txs[8+frame]);
    }
    s->setScale({scale, scale});
    s->setPosition({xpos-size/2, ypos-size/2});
    (*window).draw(*s);
}


