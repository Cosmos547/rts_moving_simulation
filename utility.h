#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "PotentialMap.h"
#include "SceneObject.h"
#include "WaterFountain.h"

PotentialMap* generateMapFromFile(std::string filename);

int manhattonDistance(int ax, int ay, int bx, int by);

void initVal(int*** a, int w, int h, int val);

void cleanVal(int*** a, int w, int h);
