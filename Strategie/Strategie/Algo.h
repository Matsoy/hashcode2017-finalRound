#pragma once
#include "Header.h"
#include "Matrix.h"

class Algo
{
private:
	std::string aType;
	Matrix & aMap;
	int aRayonRouteurs;
	int aPrixCable;
	int aPrixRouteur;
	int aBudget; // buget courant
	int aBudgetOriginal; // budget max
	int * aBackbone; // coordonnes [x, y] de l'emetteur

public:
	Algo(std::string type, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * backbone);

	// accesseurs
	std::string getType(); // type d'algo
	Matrix & getMap();
	int getRayonRouteurs();
	int getPrixCable();
	int getPrixRouteur();
	int getBudget(); // buget courant
	int getBudgetOriginal(); // budget max
	int * getBackbone(); // coordonnes [x, y] de l'emetteur

	void run();
};