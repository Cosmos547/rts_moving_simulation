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

    if (!backgroundT.loadFromFile("Assets/groundtiles.png", sf::IntRect(32, 0, 16, 16))) {
        std::cout << "Loading texture error : " << "Assets/groundtiles.png" << std::endl;
    }
    backgroundT.setSmooth(false);
    backgroundT.setRepeated(true);
    backgroundS = sf::Sprite(backgroundT);
    backgroundS.setTextureRect({0, 0, (int)p_width, (int)p_height});
    backgroundS.setPosition(0,0);



    //renderPotentialMap = true;

    //sobjs.push_back(new WaterFountain(300, 200, 200, 200));
    sobjs.push_back(new SceneTexture(100, 200, 190/3, 269/3, "Assets/tree.png"));


    fog_of_war_t.create(width, height);
    fog_of_war_t.setSmooth(false);
    fog_of_war_s = sf::Sprite(fog_of_war_t.getTexture());
    fog_of_war_s.setPosition(0, 0);
    initializeFOG();

    lightmap.setTexture(fog_of_war_t.getTexture());
    lightTexture.loadFromFile("Assets/light.png");
    lightTexture.setSmooth(true);
    light.setTexture(lightTexture);
    light.setTextureRect(sf::IntRect(0, 0, 300, 300));
    light.setOrigin(150.f, 150.f);
    light.setColor(sf::Color::White);
    light.setScale(0.5f, 0.5f);

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
    updateFOG();
    (*window).draw(fog_of_war_s, sf::BlendMultiply);
}


void PotentialMap::update(float timestep) {
    static float t = 0;
    t += timestep;
    if (t > 15.0f) t = 15.0f;
    bool er = false;
    float posi = (float)rand()/RAND_MAX;
    while (posi > (1.0 - 0.3*timestep*60) && t < 1.0f) {
        er = true;
        float bx = (float)rand()/RAND_MAX;
        bx *= 800;
        float by = (float)rand()/RAND_MAX;
        by *= 100;
        boids.push_back(new Boid(bx, by));
        posi -= 1;
        if (t > 10.0f) {
            boids.erase(boids.begin());
        }
    }
    for (auto &i : sobjs) {
        i->update(timestep);
    }
    for (auto &i : boids) {
        sf::Vector2f ipos = i->getPosition();
        float ang = atan2(gather_y - ipos.y, gather_x - ipos.x);
        float dis = sqrt(pow(gather_y - ipos.y,2) + pow(gather_x - ipos.x, 2));
        //i->calculate_forces(&boids, sf::Vector2f(cos(ang), sin(ang)));
        i->calculate_forces(&boids, this->calculatePotentialFieldForce(i->getPosition()));
        i->update(timestep);
    }



}


sf::Vector2i PotentialMap::getGridIndex(sf::Vector2f pos) {
    return sf::Vector2i(pos.x/w_pixel, pos.y/h_pixel);
}

// Takes in mouse position and set it as the destination of the movements
void PotentialMap::setDestinationGrid(sf::Vector2f pos) {
    sf::Vector2i gridlocation = getGridIndex(pos);
    if (!testValidGridIndex(gridlocation)) return;

    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            grid[i][j] = -127;
            int dis = manhattonDistance(j, i, gridlocation.x, gridlocation.y);
            int light = 128 - dis*5;
            grid[i][j] = light;
        }
    }

    for (auto &i : boids) {
        sf::Vector2i ipos = getGridIndex(i->getPosition());
        if (testValidGridIndex(ipos)) {
            grid[ipos.y][ipos.x] -= 10;
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

bool PotentialMap::testValidGridIndex(sf::Vector2i index) {
    return (index.x >= 0 && index.x < w_size && index.y >= 0 && index.y < h_size);
}


sf::Vector2f PotentialMap::calculatePotentialFieldForce(sf::Vector2f pos) {
    sf::Vector2i gridIndex = getGridIndex(pos);
    
    static int directs[16] = {-1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1};

    sf::Vector2f ret(0, 0);
    if (!testValidGridIndex(gridIndex)) {
        return ret;
    }

    int max_force = grid[gridIndex.y][gridIndex.x];
    for (int i = 0; i < 16; i+=2) {
        if (testValidGridIndex(sf::Vector2i(gridIndex.x + directs[i], gridIndex.y + directs[i+1]))) {
            if (grid[gridIndex.y + directs[i+1]][gridIndex.x + directs[i]] > max_force) {
                max_force = grid[gridIndex.y + directs[i+1]][gridIndex.x + directs[i]];
                ret.x = directs[i];
                ret.y = directs[i+1];
            }
        }
    }

    if (abs(ret.x) == 1 && abs(ret.y) == 1) {
        ret.x = ret.x / sqrt(2);
        ret.y = ret.y / sqrt(2);
    }

    return ret;


}


void PotentialMap::updateFOG() {
    fog_of_war_t.clear(sf::Color(100.0f, 100.0f, 100.0f, 180.0f));
    //fog_of_war_t.clear(sf::Color(255.f, 255.f, 255.f, 20.0f));

    for (auto &i : boids) {
        //i->renderFOG(&fog_of_war_t);
        light.setPosition(i->getPosition());
        fog_of_war_t.draw(light, sf::BlendAdd);
    }

    fog_of_war_t.display();
    lightmap.setTextureRect(sf::IntRect(0,0, (int)p_width, (int)p_height));
    lightmap.setPosition(0, 0);
    //light.setColor(sf::Color(10.0f, 10.0f, 10.0f, 180.0f));
    fog_of_war_s = lightmap;

}


void PotentialMap::initializeFOG() {
    int p_width = (float)(this->p_width);
    int p_height = (float)(this->p_height);
    static sf::Uint8* fogb = new sf::Uint8[p_width * p_height * 4];
    for (int i = 0; i < p_width; i++) {
        for (int j = 0; j < p_height; j++ ) {
            fogb[4*j*p_width + 4*i] = 0;
            fogb[4*j*p_width + 4*i + 1] = 0;
            fogb[4*j*p_width + 4*i + 2] = 0;
            fogb[4*j*p_width + 4*i + 3] = 0;
        }
    }
    //fog_of_war_t.update(fogb);
}
