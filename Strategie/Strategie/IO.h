#pragma once
#include "Header.h"
#include "Matrix.h"
#include "Cell.h"

class IO
{

public:
	IO();
	int* initializeData(std::string fichierInput);
	void initializeMap(Matrix & m, std::string fichierInput);
	int initializeMapFromSolution(Matrix & m, std::string solutionFile, std::vector<int *> & routeurs);
	void generateOutput(Matrix & mapRouteurs, std::string file);
	bool isFileExist(std::string filePath);

private:
	int counterRouter = 0;
	int counterConnectedCells = 0;
	int backboneX;
	int backboneY;
	std::string routeurtxt = "";
	std::string connectedcelltxt = "";

	void ioBrowser(Matrix & mapRouteurs, int x, int y);
};