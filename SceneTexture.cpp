#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneTexture.h"

SceneTexture::SceneTexture(float xpos, float ypos, float xsize, float ysize, std::string textureName) : SceneObject(xpos, ypos, xsize, ysize) {
    t.loadFromFile(textureName);
    rect = sf::RectangleShape(sf::Vector2f(xsize, ysize));
    rect.setPosition(xpos, ypos);
    rect.setTexture(&t);
}


void SceneTexture::render(sf::RenderWindow* window) {
    (*window).draw(rect);

}
