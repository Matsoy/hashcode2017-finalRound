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
	void generateOutput(Matrix & mapRouteurs, std::string file);
	
private:
	void ioBrowser(Matrix & mapRouteurs, int x, int y);
	std::string routeurtxt="";
	std::string connectedcelltxt = "";
	int counterRouter=0;
	int counterConnectedCells=0;
	int backboneX;
	int backboneY;
};