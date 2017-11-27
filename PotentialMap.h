#pragma once

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "SceneObject.h"
#include "Boid.h"


class PotentialMap {
    public:
        PotentialMap(float width, float height, int w, int h);
        ~PotentialMap();
        void update(float timestep);
        void render(sf::RenderWindow* window);
        void renderMinimap(sf::RenderWindow* window);
        void setRenderPotentialMap(bool b);
        void setDestinationGrid(sf::Vector2f pos);
        sf::Vector2i getGridIndex(sf::Vector2f pos);
        void setGatherPoint(float x, float y);
        sf::Vector2f calculatePotentialFieldForce(sf::Vector2f pos, int** grid);
        bool testValidGridIndex(sf::Vector2i index);
        void updateFOG();
        void calculate_obs_dir(Boid* b);
        void selectBoids(sf::Vector2f pos, sf::Vector2f size);

    private:
        float p_width;
        float p_height;
        int w_size;
        int h_size;
        float w_pixel;
        float h_pixel;
        float gather_x;
        float gather_y;
        std::vector<SceneObject*> sobjs;
        int** grid;
        int** obs_grid;
        int** shade;
        void renderGrid(sf::RenderWindow* window);
        bool renderPotentialMap;
        int pfid_count;
        sf::Texture pmapT;
        sf::Sprite pmapS;

        sf::Texture minimapT;
        sf::Sprite minimapS;


        sf::Texture backgroundT;
        sf::Sprite backgroundS;
        std::vector<Boid*> boids;


        sf::RenderTexture fog_of_war_t;
        sf::Sprite fog_of_war_s;

        sf::Texture lightTexture;
        sf::Sprite light;
        sf::Sprite lightmap;


};
