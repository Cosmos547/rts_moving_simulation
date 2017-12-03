#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "utility.h"
#include "Boid.h"
#include "PotentialField.h"


Boid::Boid(float xpos, float ypos) {

    location = sf::Vector2f(xpos, ypos);
    speed = sf::Vector2f(0,0);
    force = sf::Vector2f(0,0);
    max_speed = 20;
    max_force = 100;
    isActive = false;
    isSelected = false;
    pfid = 0;
    orientation = 270.0f;
    size = 5.0f;
    btype = 0;

}

int Boid::getBoidType() {
    return btype;
}

void Boid::setSelected(bool b) {
    isSelected = b;
}

bool Boid::getSelected() {
    return isSelected;
}

void Boid::setActive(bool b) {
    isActive = b;
}

bool Boid::getActiveState() {
    return isActive;
}

void Boid::loadPF(PotentialField* p, int pid) {
    field = p;
    pfid = pid;
}

int Boid::getPFID() {
    return pfid;
}


int** Boid::getGrid() {
    return field->getGrid();
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
    if (abs(desti.x - location.x) + abs(desti.y - location.y) < size*2) {
        isActive = false;
    }
    calculate_speed(timestep);
    update_position(timestep);

}


void Boid::limitForceDir(sf::Vector2f dir) {
    if (force.x * dir.x > 0) {
        force.x = 0;
    }
    if (speed.x * dir.x > 0) {
        speed.x = 0;
    }
    if (force.y * dir.y > 0) {
        force.y = 0;
    }
    if (speed.y * dir.y > 0) {
        speed.y = 0;
    }
}

float Boid::getSize() {
    return size;
}


void Boid::calculate_forces(std::vector<Boid*> *boids, sf::Vector2f dir) {
    float sep = 15;


    sf::Vector2f sep_force(0, 0);
    int sep_count = 0;
    float ndist = 15;
    sf::Vector2f align_force(0,0);
    sf::Vector2f cohesion_force(0, 0);
    int account = 0;
    int scount = 0;
    int ali_count = 0;
    int coh_count = 0;
    for (auto &i : *boids) {
        if (btype != i->getBoidType()) {
            continue;
        }
        float dis = this->getDistance(i);
        if (dis > 0 && dis < sep) {
            sf::Vector2f opos = i->getPosition();
            float ang = atan2(opos.y - location.y, opos.x - location.x);
            sep_force.x += -cos(ang)/dis;
            sep_force.y += -sin(ang)/dis;
            sep_count++;
        }
        if (dis > 0 && dis < ndist) {
            if (i->getPFID() == pfid) {
                if (!i->getActiveState()) {
                    account += 1;
                }
                scount += 1;
            }

            align_force += limitVector(i->getSpeed(), 1);
            ali_count ++;
        }
        if (dis > 0 && dis < ndist) {
            cohesion_force += (i->getPosition() - location)/dis;
            cohesion_force = cohesion_force;
            coh_count ++;
        }
    }

    if (sep_count > 0) {
        sep_force = sep_force/(float) sep_count;
    }

    if (account > scount/2) {
        isActive = false;
    }

    if (ali_count > 0) {
        align_force = align_force/(float) ali_count;

    }

    if (coh_count > 0) {
        cohesion_force = cohesion_force/(float)coh_count;        

    }

    force = 5.5f * sep_force;
    if (isActive) {
        force -= 2.0f * sep_force;
        force += 0.005f * align_force;
        force += 0.1f * cohesion_force;
        force += 0.3f * dir;
    }
    force *= 100.0f;


    
}


void Boid::calculate_speed(float timestep) {
    speed = speed - timestep*speed;
    speed += force*timestep;
    speed = limitVector(speed, max_speed);

    orientation = atan2(speed.y, speed.x) * 180 / 3.14159265;
    if (orientation < 0) {
        orientation += 360;
    }
}


void Boid::update_position(float timestep) {
    location += speed * timestep;
}

float Boid::getDistance(Boid* b) {
    sf::Vector2f opos = b->getPosition() - this->location;
    return sqrt(opos.x * opos.x + opos.y * opos.y);

}

void Boid::render(sf::RenderWindow* window) {
    static sf::CircleShape sh(5.0f);
    static sf::RectangleShape rect(location);
    sh.setRadius(size);
    sh.setOrigin(sh.getRadius(), sh.getRadius());
    sh.setPosition(location);
    rect.setSize(sf::Vector2f(size, size));
    rect.setOrigin(size/2, size/2);
    rect.setFillColor(sf::Color(180, 20, 50));
    if (isSelected) {
        rect.setFillColor(sf::Color(255, 255, 255, 255));
    }
    rect.setPosition(location);
    rect.setRotation(orientation);
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




FlyingBoid::FlyingBoid(float xpos, float ypos) : Boid(xpos, ypos) {
    Boid::size = 10.0f;

    t.loadFromFile("Assets/plane.png");
    t.setSmooth(false);
    s.setTexture(t);
    s.setScale({0.1, 0.1});
    btype = 1;
}


void FlyingBoid::render(sf::RenderWindow* window) {
    sf::Vertex line[2];
    line[0].position = Boid::location;
    line[1].position = {Boid::location.x, Boid::location.y-15.0f};
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;
    (*window).draw(line, 2, sf::Lines);
    s.setOrigin(s.getLocalBounds().width/2, s.getLocalBounds().height/2);
    s.setRotation(Boid::orientation + 90.0f);
    s.setPosition({location.x, location.y - 15.0f});

    (*window).draw(s);

    if (Boid::isSelected) {
        sf::CircleShape sh(size/2);
        sh.setOrigin(sh.getRadius(), sh.getRadius());
        sh.setPosition(Boid::location);
        sh.setFillColor(sf::Color::Transparent);
        sh.setOutlineThickness(1.0f);
        sh.setOutlineColor(sf::Color::Red);
        (*window).draw(sh);
    }
}
