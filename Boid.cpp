#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "utility.h"
#include "Boid.h"


Boid::Boid(float xpos, float ypos) {

    location = sf::Vector2f(xpos, ypos);
    speed = sf::Vector2f(0,0);
    force = sf::Vector2f(0,0);
    max_speed = 50;
    max_force = 100;
    isActive = false;

}


void Boid::setActive(bool b) {
    isActive = b;
}

bool Boid::getActiveState() {
    return isActive;
}


void Boid::setDestination(sf::Vector2f desti) {
    this->desti = desti;
}

sf::Vector2f Boid::limitVector(sf::Vector2f vec, float max) {
    float sz = sqrt(vec.x * vec.x + vec.y * vec.y);
    if (sz > max) {
        vec.x = vec.x * max / sz;
        vec.y = vec.y * max / sz;
    }
    return vec;
}

sf::Vector2f Boid::getPosition() {
    return location;
}

float Boid::getOrientation() {
    return atan2(speed.y, speed.x);
}

sf::Vector2f Boid::getSpeed() {
    return speed;
}

void Boid::update(float timestep) {
    if (abs(desti.x - location.x) + abs(desti.y - location.y) < 50.0f) {
        isActive = false;
    }
    calculate_speed(timestep);
    update_position(timestep);

}


void Boid::calculate_forces(std::vector<Boid*> *boids, sf::Vector2f dir) {
    pf = dir;
    force = sf::Vector2f(0, 0);
    // Calculate separation force
    float sep = 25;
    sf::Vector2f sep_force(0, 0);
    int count = 0;
    for (auto &i : *boids) {
        float dis = this->getDistance(i);
        if (dis > 0 && dis < sep) {
            sf::Vector2f opos = i->getPosition();
            float ang = atan2(opos.y - location.y, opos.x - location.x);
            sep_force.x += -cos(ang)/dis;
            sep_force.y += -sin(ang)/dis;
            count++;
        }
    }

    if (count > 0) {
        sep_force = sep_force/(float) count;
    }

    //if (abs(sep_force.x) > 0 || abs(sep_force.y) > 0) {
        //float sz = sqrt(sep_force.x * sep_force.x + sep_force.y + sep_force.y);
        //if (sep_force.x != 0) {
            //sep_force.x = sep_force.x/sz;
        //}
        //if (sep_force.y != 0) {
            //sep_force.y = sep_force.y/sz;
        //}
        //sep_force *= max_speed;
        //sep_force -= speed;
        //sep_force = limitVector(sep_force, max_force);
    
    //}


    // Calculate alignment forces
    float ndist = 50;
    sf::Vector2f align_force(0,0);
    count = 0;
    int account = 0;
    for (auto &i : *boids) {
        float dis = this->getDistance(i);
        if (dis > 0 && dis < ndist) {
            if (!i->getActiveState()) {
                account += 1;
            }
            align_force += limitVector(i->getSpeed(), 1);
            count ++;
        }
    }

    if (account > count/2) {
        isActive = false;
    }

    if (count > 0) {
        align_force = align_force/(float) count;
        //float sz = sqrt(align_force.x * align_force.x + align_force.y * align_force.y);
        //if (align_force.x != 0) {
            //align_force.x = align_force.x / sz;
        //}
        //if (align_force.y != 0) {
            //align_force.y = align_force.y / sz;
        //}
        //align_force *= max_speed;
        //align_force = align_force - speed;
        //align_force = limitVector(align_force, max_force);
    }



    // Calculate cohesion forces
    ndist = 50;
    sf::Vector2f cohesion_force(0, 0);
    count = 0;

    for (auto &i : *boids) {
        float dis = this->getDistance(i);
        if (dis > 0 && dis < ndist) {
            cohesion_force += (i->getPosition() - location)/dis;
            cohesion_force = cohesion_force;
            count ++;
        }
    }

    if (count > 0) {
        cohesion_force = cohesion_force/(float)count;        
        //float sz = sqrt(cohesion_force.x * cohesion_force.x + cohesion_force.y * cohesion_force.y);
        //if (cohesion_force.x != 0) {
            //cohesion_force.x = cohesion_force.x/sz;
        //}
        //if (cohesion_force.y != 0) {
            //cohesion_force.y = cohesion_force.y/sz;
        //}
        //cohesion_force *= max_speed;
        //cohesion_force = cohesion_force - speed;
        //cohesion_force = limitVector(cohesion_force, max_force);
    }


    // Calculate Destination force
    sf::Vector2f dest_force = dir;
    //dest_force *= max_speed;
    //dest_force = dest_force - speed;
    //dest_force = limitVector(dest_force, max_force);


    force += 1.5f * sep_force;
    if (isActive) {
        force += 4.0f * sep_force;
        force += 0.005f * align_force;
        force += 0.1f * cohesion_force;
        force += 0.05f * dest_force;
    }
    force *= 1000.0f;


    
}


void Boid::calculate_speed(float timestep) {
    speed = speed - timestep*speed;
    speed += force*timestep;
    speed = limitVector(speed, max_speed);
}


void Boid::update_position(float timestep) {
    location += speed * timestep;
}

float Boid::getDistance(Boid* b) {
    sf::Vector2f opos = b->getPosition() - this->location;
    return sqrt(opos.x * opos.x + opos.y * opos.y);

}

void Boid::renderFOG(sf::RenderTexture* t) {

    //sf::Texture lightTexture;
    //sf::Sprite light;
    //sf::Sprite lightmap;

    //lightmap.setTexture((*t).getTexture());
    //lightTexture.loadFromFile("Assets/light.png");
    //lightTexture.setSmooth(true);

    //light.setTexture(lightTexture);
    //light.setTextureRect(sf::IntRect(0, 0, 50, 50));
    //light.setOrigin(25.f, 25.f);
    //light.setColor(sf::Color::White);
    //light.setPosition(location);
    //(*t).draw(light, sf::BlendAdd);

}


void Boid::render(sf::RenderWindow* window) {
    static sf::CircleShape sh(5.0f);
    static sf::RectangleShape rect(location);
    sh.setRadius(5.0f);
    sh.setOrigin(sh.getRadius(), sh.getRadius());
    sh.setPosition(location);
    rect.setSize(sf::Vector2f(5.0f, 5.0f));
    rect.setOrigin(2.5f, 2.5f);
    rect.setFillColor(sf::Color(180, 20, 50));
    rect.setPosition(location);
    (*window).draw(rect);
    sf::Vertex line[2];
    line[0].position = location;
    line[1].position = location + speed/5.0f;
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;
    (*window).draw(line, 2, sf::Lines);
    //sh.setPointCount(50);
    //sh.setOutlineThickness(1.0f);
    //sh.setFillColor(sf::Color(180,20,50,0));
    //sh.setOutlineColor(sf::Color(180,20,50, 90));
    //sh.setRadius(50.0f);
    //sh.setOrigin(sh.getRadius(), sh.getRadius());
    //(*window).draw(sh);
    //sh.setRadius(25.0f);
    //sh.setOrigin(sh.getRadius(), sh.getRadius());
    //(*window).draw(sh);

}

