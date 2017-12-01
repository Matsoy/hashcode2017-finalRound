#pragma once
#include "Header.h"
#include "Matrix.h"

class Algo
{
private:
	std::string aMethod;
	Matrix & aMap;
	int aRayonRouteurs;
	int aPrixCable;
	int aPrixRouteur;
	int aBudget; // budget courant
	int aBudgetOriginal; // budget max
	int * aBackbone; // coordonnes [x, y] de l'emetteur
	int aNbCellsOriginal; // nombre de cellules cibles au depart

public:
	Algo(std::string method, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * backbone);

	// accesseurs
	std::string getMethod(); // type d'algo
	Matrix & getMap();
	int getRayonRouteurs();
	int getPrixCable();
	int getPrixRouteur();
	int getBudget(); // budget courant
	int getBudgetOriginal(); // budget max
	int * getBackbone(); // coordonnes [x, y] de l'emetteur
	int getNbCellsOriginal(); // nombre de cellules cibles au depart

	void run();
	void kruskal(Matrix & m, int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool & succ, int & cost);
	int chessboardDist(int * routeur, int * newrouteur);
	int quasiEuclideanDist(int * routeur, int * newrouteur);
	void toCsrMatrix(Matrix & mat, std::vector<int> & dists, std::vector<int> & idx, std::vector<int> & idy, int dim);
	void toMinimumSpanningTree(Matrix & csrMat, Matrix & mstMat);
	int minKey(std::vector<int> key, std::vector<bool> mstSet, int dim);
	void random();
	void wirelessAccess(int x, int y, int radius, Matrix & mat, Matrix & mask);
	void placeMstPaths(std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists);
	int argMin(int * tab, int size);
	void findChessConnection(int * Routerfrom, int * RouterTo, std::vector<int *> & cables);
	bool routerOnBackbone(std::vector<int *> & routeurs);
	int cellsCoveredPercentage(Matrix & targetCells);
	bool isCyclical(Matrix & mat);
	int nbCellsCovered(Matrix & targetCells);
};