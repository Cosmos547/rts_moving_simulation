#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include "SceneObject.h"
#include "PotentialMap.h"
#include "WaterFountain.h"


PotentialMap::PotentialMap(float width, float height, int w, int h) : p_width(width), p_height(height), w_size(w), h_size(h) {
    sobjs.push_back(new WaterFountain(300, 200, 200, 200));
    w_pixel = width/w;
    h_pixel = height/h;
    grid = new int*[h];
    for(int i = 0; i < h; i++) {
        grid[i] = new int[w];
        for (int j = 0; j < w; j++) {
            grid[i][j] = 0;
        }
    }
}

PotentialMap::~PotentialMap() {
    free(grid);
}

void PotentialMap::renderGrid(sf::RenderWindow* window) {
    sf::RectangleShape rect(sf::Vector2f(w_pixel ,h_pixel));
    rect.setFillColor(sf::Color(0, 179, 0));
    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            rect.setPosition(j*w_pixel, i*h_pixel);
            (*window).draw(rect);
        }
    }

}

void PotentialMap::render(sf::RenderWindow* window) {
    renderGrid(window);
    for (auto &i : sobjs) {
        i->render(window);
    }
}


void PotentialMap::update(float timestep) {
    for (auto &i : sobjs) {
        i->update(timestep);
    }
}
