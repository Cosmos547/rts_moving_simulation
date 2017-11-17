#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "WaterFountain.h"
#include "SceneObject.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::Clock clock;
    float elapsedTime;
    WaterFountain wf = WaterFountain(300, 200, 300, 300);

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
        wf.update(elapsedTime);
        window.setTitle("FPS: " + std::to_string(1.0f/elapsedTime));

        window.clear();
        wf.render(&window);
        window.display();
    }

    return 0;
}
