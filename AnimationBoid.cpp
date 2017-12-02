#include <SFML/Graphics.hpp>
#include "Boid.h"
#include "Animation.h"
#include "AnimationBoid.h"


AnimationBoid::AnimationBoid(float xpos, float ypos, Animation* a) : Boid(xpos, ypos), ani(a) {
    Boid::size = 20;
}

void AnimationBoid::update(float timestep) {
    Boid::update(timestep);
    ani->update(timestep);
}


void AnimationBoid::render(sf::RenderWindow* window) {
    ani->render(window, Boid::location.x, Boid::location.y, Boid::orientation, Boid::size, Boid::speed);
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
