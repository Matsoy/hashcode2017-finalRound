#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <math.h>

std::vector<int> infoMap(const char * mapName);
std::map<int, std::vector<int>> detectMurs(const char * mapName);
bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::pair<int, int>> & cableVector);
bool cableOrdered2(const int & xCable, const int & yCable, const std::map<int, std::vector<int>> & cablesMap);
bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected);
bool checkTest(const char* solutionName);
