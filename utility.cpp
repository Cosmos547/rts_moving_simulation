#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <iterator>
#include <algorithm>
#include "PotentialMap.h"
#include "SceneObject.h"
#include "WaterFountain.h"
#include "utility.h"

const int MAXLINE = 256;

PotentialMap* generateMapFromFile(std::string filename) {
    std::ifstream inFile (filename);
    std::string oneline;
    PotentialMap* pf = new PotentialMap(800, 600, 80, 60);

    if (inFile) {
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        inFile.close();
        
        while (getline(buffer, oneline)) {
            if (oneline.empty() || oneline[0] == '#') {
                continue;
            } else {
                std::cout << oneline << std::endl;
                std::istringstream iss(oneline);
                std::vector<std::string> tokens = split(oneline, ' ');
                for (auto &i : tokens) {
                    std::cout << i << std::endl;
                }

                if (tokens[0] == "grid") {
                    pf = new PotentialMap(std::atoi(tokens[1].c_str()), std::atoi(tokens[2].c_str()), std::atoi(tokens[3].c_str()), std::atoi(tokens[4].c_str()));
                }
                
            }
        }
    }


    return pf;
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


// https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
