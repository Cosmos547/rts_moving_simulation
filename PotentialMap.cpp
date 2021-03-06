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
#include "Animation.h"
#include "AnimationBoid.h"


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

    shade = new float*[h];
    for(int i = 0; i < h; i++) {
        shade[i] = new float[w];
        for (int j = 0; j < w; j++) {
            shade[i][j] = 0.0f;
        }
    }

    obs_grid = new int*[h];
    for(int i = 0; i < h; i++) {
        obs_grid[i] = new int[w];
        for (int j = 0; j < w; j++) {
            obs_grid[i][j] = 0;
        }
    }

    for (int i = 0; i < h; i++) {
        obs_grid[i][0] = 1;
        obs_grid[i][w-1] = 1;
    }

    for (int i = 0; i < w; i++) {
        obs_grid[0][i] = 1;
        obs_grid[h-1][i] = 1;
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


    pfid_count = 1;



    renderPotentialMap = false;



    fog_of_war_t.create(width, height);
    fog_of_war_t.setSmooth(false);
    fog_of_war_s = sf::Sprite(fog_of_war_t.getTexture());
    fog_of_war_s.setPosition(0, 0);

    lightmap.setTexture(fog_of_war_t.getTexture());
    lightTexture.loadFromFile("Assets/light.png");
    lightTexture.setSmooth(true);
    light.setTexture(lightTexture);
    light.setTextureRect(sf::IntRect(0, 0, 300, 300));
    light.setOrigin(150.f, 150.f);
    light.setColor(sf::Color::White);
    light.setScale(0.5f, 0.5f);
}

PotentialMap::~PotentialMap() {
    free(grid);
}

void PotentialMap::togglePotentialMap() {
    renderPotentialMap = !renderPotentialMap;
}

void PotentialMap::renderGrid(sf::RenderWindow* window) {
    static sf::Uint8* pixels = new sf::Uint8[w_size * h_size * 4];
    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            pixels[i*w_size*4 + 4*j] = 0;
            if (grid[i][j] + shade[i][j] >= -127){
                pixels[i*w_size*4 + 4*j+1] = 127 + grid[i][j] + shade[i][j];
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

    for (auto &i : sobjs) {
        i->update(timestep);
    }
    for (auto &i : boids) {
        if (i->getPFID() != 0) {
            i->calculate_forces(&boids, this->calculatePotentialFieldForce(i->getPosition(),i->getGrid()));
        } else {
            i->calculate_forces(&boids, this->calculatePotentialFieldForce(i->getPosition(), obs_grid));
        }
        calculate_obs_dir(i);
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
            int dis = (j - gridlocation.x)*(j-gridlocation.x) + (i-gridlocation.y) *(i-gridlocation.y);
            int light = 128 - dis/5;
            grid[i][j] = light;
        }
    }

    for (auto &i : boids) {
        if (i->getSelected()) {
            i->setActive(true);
            i->setDestination(pos);
        }
    }


    PotentialField* cur_pf = new PotentialField(w_size, h_size);
    cur_pf->loadGrid(grid);
    cur_pf->loadShade(shade);
    cur_pf->generatePF();

    PotentialField* air_pf = new PotentialField(w_size, h_size);
    air_pf->loadGrid(grid);
    air_pf->generatePF();

    for (auto &i : boids) {
        if (i->getSelected()) {
            if (i->getBoidType() == 0) {
                i->loadPF(cur_pf, pfid_count);
            }
            if (i->getBoidType() == 1) {
                i->loadPF(air_pf, pfid_count+1);
            }
        }
    }
    pfid_count += 2;


    if (pfid_count > 10000) {
        pfid_count = 1;
    }

}


void PotentialMap::generateShade() {
    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            if (obs_grid[i][j] == 1) {
                for (int ii = 0; ii < h_size; ii++) {
                    for (int jj = 0; jj < w_size; jj++) {
                        int dis = (jj-j)*(jj-j) + (ii - i)* (ii - i);
                        if (dis > 0 && dis < 25) {
                            shade[ii][jj] -= 80.0f/dis;
                        }
                        if (dis == 0) {
                            shade[ii][jj] -= 80;
                        }
                    }
                }
            }
        }
    }


}


void PotentialMap::renderMinimap(sf::RenderWindow* window) {
    static sf::Uint8* pixels = new sf::Uint8[w_size * h_size * 4];
    for (int i = 0; i < h_size; i++) {
        for (int j = 0; j < w_size; j++) {
            if (obs_grid[i][j] == 1){
                pixels[i*w_size*4 + 4*j] = 255;
                pixels[i*w_size*4 + 4*j+1] = 255;
                pixels[i*w_size*4 + 4*j+2] = 255;
            } else {
                pixels[i*w_size*4 + 4*j] = 0;
                pixels[i*w_size*4 + 4*j+1] = 127;
                pixels[i*w_size*4 + 4*j+2] = 0;
            }
            pixels[i*w_size*4 + 4*j+3] = 255;
        }
    }
    for (auto &i : boids) {
        sf::Vector2i pp = getGridIndex(i->getPosition());
        pixels[pp.y*w_size*4 + 4*pp.x] = 255;
        pixels[pp.y*w_size*4 + 4*pp.x + 1] = 0;
        pixels[pp.y*w_size*4 + 4*pp.x + 2] = 0;
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


sf::Vector2f PotentialMap::calculatePotentialFieldForce(sf::Vector2f pos, int** grid) {
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



void PotentialMap::calculate_obs_dir(Boid* b) {
    sf::Vector2i gridIndex = getGridIndex(b->getPosition());
    static int directs[16] = {-1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1};
    if (!testValidGridIndex(gridIndex)) {
        return;
    }
    for (int i = 0; i < 16; i+=2) {
        if (testValidGridIndex(sf::Vector2i(gridIndex.x + directs[i], gridIndex.y + directs[i+1]))) {
            if (obs_grid[gridIndex.y + directs[i+1]][gridIndex.x + directs[i]] == 1 && b->getBoidType() == 0) {
                b->limitForceDir(sf::Vector2f(directs[i], directs[i+1]));
            }
        }
    }
}


void PotentialMap::updateFOG() {
    fog_of_war_t.clear(sf::Color(100.0f, 100.0f, 100.0f, 180.0f));

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



void PotentialMap::selectBoids(sf::Vector2f pos, sf::Vector2f size) {

    for (auto &i : boids) {
        i->setSelected(false);
    
        sf::Vector2f ipos = i->getPosition();
        if (ipos.x > pos.x && ipos.y > pos.y && ipos.x - pos.x < size.x && ipos.y - pos.y < size.y) {
        
            i->setSelected(true);
        }
    }

}


void PotentialMap::setObs(float x, float y, float w, float h) {
    sf::Vector2i tlg = getGridIndex({x, y});
    sf::Vector2i brg = getGridIndex({x+w, y+h});

    for (int i = tlg.x; i < brg.x; i++) {
        for (int j = tlg.y; j < brg.y; j++) {
            obs_grid[j][i] = 1;
        }
    }

}
