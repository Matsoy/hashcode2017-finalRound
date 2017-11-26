#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>


std::vector<int> infoMap(const char * mapName);
bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected);
bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::vector<bool>> & cablesVector);
std::vector<std::vector<bool>> detectWalls(const char * mapName, const int & row, const int & column);
bool checkTest(const char* input, const char* solutionName);
