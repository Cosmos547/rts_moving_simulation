#include <SFML/Graphics.hpp>
#include <iostream>
#include "PotentialMap.h"
#include "SceneObject.h"
#include "WaterFountain.h"

PotentialMap* generateMapFromFile(std::string filename) {
    return new PotentialMap(800, 600, 160, 120);
}
