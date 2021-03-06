#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>
#include <map>

struct Cell
{
	enum type {
		Void = -1,	//Cellule vide
		Wall = 0,	//Mur 
		Wireless = 1, //Cellule �Econnecter
		Connected = 2 //Cellule connect�e
	};
};

std::vector<int> infoMap(const std::string & mapName);
bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected);
bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::vector<bool>> & cablesVector);
std::vector<std::vector<int>> parseMap(const std::string & mapName, const int & row, const int & column);
int checkSolution(const std::string & solutionFile);
int scoreRouter(std::vector<std::vector<int>> & walls, const std::vector<std::pair<int, int>> & routerCoord, const int & routerRadius, const int & row, const int & column);
bool wallPresence(const std::vector<std::vector<int>> & walls, const int & xRouter, const int & yRouter, const int & xCell, const int & yCell);
std::string remove_extension(const std::string& filename);
void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory,const std::string filter);
void CleanupDirectory(const std::string &directory);