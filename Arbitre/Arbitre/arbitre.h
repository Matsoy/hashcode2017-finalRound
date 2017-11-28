#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

std::vector<int> infoMap(const std::string & mapName);
bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected);
bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::vector<bool>> & cablesVector);
std::vector<std::vector<bool>> detectWalls(const std::string & mapName, const int & row, const int & column);
bool checkSolution(const std::string & mapFile, const std::string & solutionFile);
int scoreRouter(const std::vector<std::vector<bool>> & walls, const std::vector<std::pair<int, int>> & routerCoord, const int & routerRadius, const int & row, const int & column);
bool wallPresence(const std::vector<std::vector<bool>> & walls, const int & xRouter, const int & yRouter, const int & xCell, const int & yCell);
