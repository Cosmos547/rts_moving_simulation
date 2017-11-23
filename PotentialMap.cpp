#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include "SceneObject.h"
#include "PotentialMap.h"
#include "WaterFountain.h"
#include "SceneTexture.h"
#include "utility.h"
#include "Boid.h"
#include <time.h>


PotentialMap::PotentialMap(float width, float height, int w, int h) : p_width(width), p_height(height), w_size(w), h_size(h) {
    w_pixel = width/w;
    h_pixel = height/h;
    grid = new int*[h];
    for(int i = 0; i < h; i++) {
        grid[i] = new int[w];
        for (int j = 0; j < w; j++) {
            grid[i][j] = 0;
        }
    }


    srand(time(NULL));

    pmapT.create(w, h);
    pmapT.setSmooth(false);
    pmapS.setTexture(pmapT);

    pmapS.setScale(w_pixel, h_pixel);
    pmapS.setPosition(0, 0);



    minimapT.create(w, h);
    minimapT.setSmooth(false);
    minimapS.setTexture(minimapT);
    minimapS.setPosition(4*(int)p_width/5, 4*(int)p_height/5);
    minimapS.setScale((int)p_width/5/w, (int)p_height/5/h);

    //backgroundT.setRepeated(true);
    if (!backgroundT.loadFromFile("Assets/groundtiles.png", sf::IntRect(32, 0, 16, 16))) {
        std::cout << "Loading texture error : " << "Assets/groundtiles.png" << std::endl;
    }
    backgroundT.setSmooth(false);
    backgroundT.setRepeated(true);
    backgroundS = sf::Sprite(backgroundT);
    backgroundS.setTextureRect({0, 0, (int)p_width, (int)p_height});
    backgroundS.setPosition(0,0);



    renderPotentialMap = false;

    //sobjs.push_back(new WaterFountain(300, 200, 200, 200));
    sobjs.push_back(new SceneTexture(100, 200, 190/3, 269/3, "Assets/tree.png"));



    boids.push_back(new Boid(200, 100));
    boids.push_back(new Boid(210, 100));
    boids.push_back(new Boid(220, 100));
    boids.push_back(new Boid(230, 100));
    boids.push_back(new Boid(240, 100));
    boids.push_back(new Boid(250, 100));
    boids.push_back(new Boid(260, 100));
    boids.push_back(new Boid(270, 100));
    boids.push_back(new Boid(280, 100));
    boids.push_back(new Boid(290, 100));
    boids.push_back(new Boid(200, 110));
    boids.push_back(new Boid(200, 120));
    boids.push_back(new Boid(200, 130));
    boids.push_back(new Boid(200, 140));
    boids.push_back(new Boid(200, 150));
    boids.push_back(new Boid(200, 160));
    boids.push_back(new Boid(200, 170));
    boids.push_back(new Boid(200, 180));



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
    (*window).draw(backgroundS);
    if (renderPotentialMap) {
        renderGrid(window);
    }
    for (auto &i : sobjs) {
        i->render(window);
    }
    for (auto &i : boids) {
        i->render(window);
    }
}


void PotentialMap::update(float timestep) {
    static float t = 0;
    t += timestep;
    if (t > 15.0f) t = 15.0f;
    bool er = false;
    if ((float)rand()/RAND_MAX < 0.3*timestep*60) {
        er = true;
        float bx = (float)rand()/RAND_MAX;
        bx *= 800;
        float by = (float)rand()/RAND_MAX;
        by *= 100;
        boids.push_back(new Boid(bx, by));
    }
    for (auto &i : sobjs) {
        i->update(timestep);
    }
    for (auto &i : boids) {
        sf::Vector2f ipos = i->getPosition();
        float ang = atan2(gather_y - ipos.y, gather_x - ipos.x);
        float dis = sqrt(pow(gather_y - ipos.y,2) + pow(gather_x - ipos.x, 2));
        //i->calculate_forces(&boids, 0, 1);
        i->calculate_forces(&boids, cos(ang), sin(ang));
        //i->calculate_forces(&boids, 0, 0);
        i->update(timestep);
    }

    if (t > 10.0f && !boids.empty() && er) {
        boids.erase(boids.begin());
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


void PotentialMap::renderMinimap(sf::RenderWindow* window) {
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
    minimapT.update(pixels);
    (*window).draw(minimapS);

}


void PotentialMap::setGatherPoint(float x, float y) {
    this->gather_x = x;
    this->gather_y = y;
}
