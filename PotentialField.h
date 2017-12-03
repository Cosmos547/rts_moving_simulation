#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>

class PotentialField {
    public:
        PotentialField(int w, int h);
        ~PotentialField();
        void loadShade(float** in_shade);
        void loadObs(int** in_shade);
        void loadGrid(int** in_shade);
        void generatePF();
        int** getGrid();

    private:
        int w;
        int h;
        int** pf;
        int** grid;
        int** obs;
        int** shade;


};

