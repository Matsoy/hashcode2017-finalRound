#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

std::vector<int> infoMap(const char * mapName);
std::map<int, std::vector<int>> detectMurs(const char * mapName);
bool checkTest(const char* solutionName);