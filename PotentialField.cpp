#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "PotentialField.h"
#include "utility.h"


PotentialField::PotentialField(int w, int h) : w(w), h(h) {
    initVal(&pf, w, h, 0);
    initVal(&grid, w, h, 0);
    initVal(&obs, w, h, 0);
    initVal(&shade, w, h, 0);
}

PotentialField::~PotentialField() {
    cleanVal(&pf, w, h);
    cleanVal(&grid, w, h);
    cleanVal(&obs, w, h);
    cleanVal(&shade, w, h);
}


int** PotentialField::getGrid() {
    return pf;
}

void PotentialField::loadShade(int** in) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            shade[i][j] = in[i][j];
        }
    }
}

void PotentialField::loadGrid(int** in) {
     for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            grid[i][j] = in[i][j];
        }
    }   
}


void PotentialField::loadObs(int** in) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            obs[i][j] = in[i][j];
        }
    }
}


void PotentialField::generatePF() {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            pf[i][j] += grid[i][j] + obs[i][j] + shade[i][j];
        }
    }
}

