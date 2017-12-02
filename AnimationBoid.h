#include <SFML/Graphics.hpp>
#include "Boid.h"
#include "Animation.h"


class AnimationBoid : public Boid {
    public:
        AnimationBoid(float xpos, float ypos, Animation* a);
        void update(float timestep);
        void render(sf::RenderWindow* window);

    private:
        Animation* ani;
};
