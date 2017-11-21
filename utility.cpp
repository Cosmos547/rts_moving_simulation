#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "PotentialMap.h"
#include "SceneObject.h"
#include "WaterFountain.h"

PotentialMap* generateMapFromFile(std::string filename) {
    return new PotentialMap(800, 600, 80, 60);
}


int manhattonDistance(int ax, int ay, int bx, int by) {
    return abs(ax - bx) + abs(ay - by);
}
