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
    window.setVerticalSyncEnabled(true);
    sf::View view(sf::FloatRect(0,0,800,600));
    bool middleMouseDown = false;
    sf::Vector2f preMouse;
    
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2f mpos = window.mapPixelToCoords(pos, view);
                    std::cout << "cur mouse: " << mpos.x << " " << mpos.y << std::endl;
                    std::cout << "pre mouse: " << preMouse.x << " " << preMouse.y << std::endl;
                    std::cout << "Mous delta: " << (mpos - preMouse).x << " " << (mpos - preMouse).y << std::endl;
                    view.move(preMouse - mpos);
                }
                //std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
                //std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
                //view.move((float)event.mouseMove.x, (float)event.mouseMove.y);
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                view.zoom(1.0 + 0.05*event.mouseWheel.delta);
                //std::cout << "Wheel movement: " << event.mouseWheel.delta << std::endl;
                //std::cout << "mouse x: " << event.mouseWheel.x << std::endl;
                //std::cout << "mouse y: " << event.mouseWheel.y << std::endl;
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }


        
        


        window.setView(view);
        
        sf::Vector2i position = sf::Mouse::getPosition(window);
        preMouse = window.mapPixelToCoords(position, view);
        pm->setDestinationGrid(preMouse);
        position = pm->getGridIndex(preMouse);
        //std::cout << "Current position: " << position.x << " " << position.y << std::endl;


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
