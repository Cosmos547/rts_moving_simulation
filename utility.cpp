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

void initVal(int*** a, int w, int h, int val) {
    *a = new int*[h];
    for (int i = 0; i < h; i++) {
        (*a)[i] = new int[w];
        for (int j = 0; j < w; j++) {
            (*a)[i][j] = val;
        }
    }
}


void cleanVal(int*** a, int w, int h) {
    for (int i = 0; i < h; i++) {
        free((*a)[i]);
    }
    free((*a));
}
