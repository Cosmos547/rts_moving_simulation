#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "WaterFountain.h"
#include "SceneObject.h"
#include <iostream>
#include "PotentialMap.h"
#include "utility.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::Clock clock;
    float elapsedTime;
    PotentialMap* pm = generateMapFromFile("map.txt");
    


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        pm->update(elapsedTime);
        window.setTitle("FPS: " + std::to_string(1.0f/elapsedTime));

        window.clear();
        pm->render(&window);
        window.display();

        // Code to limit framerate for testing purpose
        //while (clock.getElapsedTime().asSeconds() < 0.01) {
        //}
    }

    return 0;
}
