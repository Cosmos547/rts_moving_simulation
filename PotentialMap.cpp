#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include "SceneObject.h"
#include "PotentialMap.h"
#include "WaterFountain.h"
#include "utility.h"


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

    pmapT.create(w, h);
    pmapT.setSmooth(false);
    pmapS.setTexture(pmapT);

    pmapS.setScale(w_pixel, h_pixel);
    pmapS.setPosition(0, 0);

    renderPotentialMap = false;
}

PotentialMap::~PotentialMap() {
    free(grid);
}

void PotentialMap::renderGrid(sf::RenderWindow* window) {
    static sf::Uint8* pixels = new sf::Uint8[w_size * h_size * 4];
    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            pixels[i*w_size*4 + 4*j] = 0;
            if (grid[i][j] >= -127){
                pixels[i*w_size*4 + 4*j+1] = 127 + grid[i][j];
            } else {
                pixels[i*w_size*4 + 4*j+1] = 0;
            }
            pixels[i*w_size*4 + 4*j+2] = 0;
            pixels[i*w_size*4 + 4*j+3] = 255;
        }
    }
    pmapT.update(pixels);
    (*window).draw(pmapS);
}

void PotentialMap::render(sf::RenderWindow* window) {
    if (renderPotentialMap) {
        renderGrid(window);
    }
    for (auto &i : sobjs) {
        i->render(window);
    }
}


void PotentialMap::update(float timestep) {
    for (auto &i : sobjs) {
        i->update(timestep);
    }
}


sf::Vector2i PotentialMap::getGridIndex(sf::Vector2f pos) {
    return sf::Vector2i(pos.x/w_pixel, pos.y/h_pixel);
}

// Takes in mouse position and set it as the destination of the movements
void PotentialMap::setDestinationGrid(sf::Vector2f pos) {
    sf::Vector2i gridlocation = getGridIndex(pos);
    if (gridlocation.x < 0 || gridlocation.x >= w_size || gridlocation.y < 0 || gridlocation.y >= h_size) return;
    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            grid[i][j] = -127;
            int dis = manhattonDistance(j, i, gridlocation.x, gridlocation.y);
            int light = 128 - dis*5;
            grid[i][j] = light;
        }
    }
}
