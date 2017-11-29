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
	void generateOutput(Matrix & mapRouteurs);
	void connectedCells(Matrix & mapRouteurs, int x, int y);

private:
	std::string routeurtxt="";
	std::string connectedcelltxt = "";
	int counterRouter=0;
	int counterConnectedCells=0;
	int backboneX;
	int backboneY;
};