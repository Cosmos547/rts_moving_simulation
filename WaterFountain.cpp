#include <SFML/Graphics.hpp>

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>

#include "WaterFountain.h"

WaterFountain::WaterFountain(float xpos, float ypos, float xsize, float ysize) : SceneObject(xpos, ypos, xsize, ysize) {

    this->generationRate = 0.5;
    srand(time(NULL));

    this->x_cen = xpos + xsize/2.0f;
    this->y_cen = ypos + ysize/2.0f;
    this->radius = xsize > ysize ? ysize/2.0f : xsize/2.0f;
    std::cout << "Radius of fountain: " << radius << std::endl;
    this->pt_speed = radius/5.0f;
    this->gravity = pt_speed/2.0f;
    this->cir = sf::CircleShape(1.0f);
}


void WaterFountain::render(sf::RenderWindow* window) {
    // Render the base of the water fountain
    this->cir.setPointCount(50);
    this->cir.setPosition(x_cen-radius/4.0f, y_cen);
    this->cir.setFillColor(sf::Color(102, 204, 255));
    this->cir.setScale(1.0, 0.8);
    this->cir.setRadius(radius/4.0);
    (*window).draw(cir);

    // Render the individual particles
    this->cir.setPointCount(3);
    this->cir.setFillColor(sf::Color::Blue);
    this->cir.setRadius(1.0f);
    this->cir.setScale(1.0, 1.0);
    for (auto &i : particles) {
        cir.setPosition(i[0]+x_cen, i[1]+y_cen);
        (*window).draw(cir);
    }

}

void WaterFountain::update(float time) {
    float rd = (float)rand()/RAND_MAX;
    // Model the generation rate as generating a random number between 0 and 1 then if it's below the generation rate take actions.
    if (rd < generationRate) {
        float* random_pt = generateRandomParticle();
        particles.push_back(random_pt);
    }

    // Updates all the particles in the system
    for (auto &i : particles) {
        updateParticle(i, time);
    }

    removeDeprecateParticle();
    
}

void WaterFountain::removeDeprecateParticle() {
    for (int i = 0; i < particles.size(); i++) {
        if (particles[i][4] > radius/pt_speed/2.0f) {
            particles.erase(particles.begin() + i);
            i--;
        }
    }

}

float* WaterFountain::generateRandomParticle() {

    float angle = (float)rand()/RAND_MAX;
    angle *= 360.0f;
    float yaw = (float)rand()/RAND_MAX;
    yaw *= 40;
    yaw += 50;
    float *pt = new float[5];
    pt[0] = 0.0f;
    pt[1] = 0.0f;
    pt[2] = angle;
    pt[3] = yaw;
    pt[4] = 0.0f;
    return pt;
}

void WaterFountain::updateParticle(float* pt, float time) {
    float cy = cos(pt[3]);
    pt[4] += time;
    pt[0] += cos(pt[2])*pt_speed*cy*time;
    pt[1] -= sin(pt[2])*pt_speed*cy*time;
    pt[1] += this->gravity*time;

}
