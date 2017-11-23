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

    //window.setVerticalSyncEnabled(true);

    sf::View view(sf::FloatRect(0,0,800,600));
    sf::View minimap(sf::FloatRect(0, 0, 800, 600));
    bool middleMouseDown = false;
    sf::Vector2f preMouse;
    sf::Vector2f holdMouseS;
    sf::Vector2f holdMouseE;
    bool leftMouseDown = false;
    
    
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseMoved) {
                // Action for dragging the mouse with middle button down to move around the map
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2f mpos = window.mapPixelToCoords(pos, view);

                    //std::cout << "cur mouse: " << mpos.x << " " << mpos.y << std::endl;
                    //std::cout << "pre mouse: " << preMouse.x << " " << preMouse.y << std::endl;
                    //std::cout << "Mous delta: " << (mpos - preMouse).x << " " << (mpos - preMouse).y << std::endl;
                    view.move(preMouse - mpos);
                }
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    holdMouseE = window.mapPixelToCoords(pos, view);
                    
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


        
        


        
        sf::Vector2i position = sf::Mouse::getPosition(window);
        preMouse = window.mapPixelToCoords(position, view);

        pm->setDestinationGrid(preMouse);
        pm->setGatherPoint(preMouse.x, preMouse.y);

        position = pm->getGridIndex(preMouse);
        //std::cout << "Current position: " << position.x << " " << position.y << std::endl;


        elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        pm->update(elapsedTime);
        window.setTitle("FPS: " + std::to_string(1.0f/elapsedTime));

        window.clear();
        window.setView(view);
        pm->render(&window);
        

        // Draw dragging box
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (!leftMouseDown) {
                holdMouseS = preMouse;
                holdMouseE = holdMouseS;
            }
            //std::cout << "Left button pressed" << std::endl;
            //std::cout << "Start mouse position " << holdMouseS.x << " " << holdMouseS.y << std::endl;
            //std::cout << "End mouse position " << holdMouseE.x << " " << holdMouseE.y << std::endl;
            //std::cout << "Left mouse condition: " << leftMouseDown << std::endl;
            leftMouseDown = true;
            sf::RectangleShape rect(sf::Vector2f(abs(holdMouseE.x - holdMouseS.x), abs(holdMouseE.y - holdMouseS.y)));
            rect.setPosition(sf::Vector2f(holdMouseE.x < holdMouseS.x ? holdMouseE.x : holdMouseS.x, holdMouseE.y < holdMouseS.y ? holdMouseE.y : holdMouseS.y));
            rect.setFillColor(sf::Color(102, 204, 255, 0));
            rect.setOutlineThickness(2.0);
            rect.setOutlineColor(sf::Color(102, 204, 255, 95));
            window.draw(rect);
        } else {
            leftMouseDown = false;
        }

        window.setView(minimap);
        pm->renderMinimap(&window);


        window.display();

        // Code to limit framerate for testing purpose
        //while (clock.getElapsedTime().asSeconds() < 0.01) {
        //}
    }

    return 0;
}
