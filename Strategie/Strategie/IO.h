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
	void generateInput(Matrix & mapRouteurs);
};