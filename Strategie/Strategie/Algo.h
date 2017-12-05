#pragma once
#include "Header.h"
#include "Matrix.h"

class Algo
{
private:
	std::string aMethod;
	std::string aMapName;
	Matrix & aMap;
	int aRayonRouteurs;
	int aPrixCable;
	int aPrixRouteur;
	int aBudget; // budget courant
	int aBudgetOriginal; // budget max
	int * aBackbone; // coordonnes [x, y] de l'emetteur
	int aNbCellsOriginal; // nombre de cellules cibles au depart
	int aBestScore; // meilleur score de la carte correspondante

public:
	Algo(std::string method, std::string mapName, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * backbone);

	// accesseurs
	std::string getMethod(); // type d'algo
	std::string getMapName(); // nom de la carte
	Matrix & getMap();
	int getRayonRouteurs();
	int getPrixCable();
	int getPrixRouteur();
	int getBudget(); // budget courant
	int getBudgetOriginal(); // budget max
	int * getBackbone(); // coordonnes [x, y] de l'emetteur
	int getNbCellsOriginal(); // nombre de cellules cibles au depart

	void run(int bestScore);
	void kruskal(Matrix & m, int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool & succ, int & cost);
	int chessboardDist(int * routeur, int * newrouteur);
	int quasiEuclideanDist(int * routeur, int * newrouteur);
	void toCsrMatrix(Matrix & mat, std::vector<int> & dists, std::vector<int> & idx, std::vector<int> & idy, int dim);
	void toMinimumSpanningTree(Matrix & csrMat, Matrix & mstMat);
	int minKey(std::vector<int> key, std::vector<bool> mstSet, int dim);
	void random();
	void bigCase();
	void gaussianBlur();
	void wirelessAccess(int x, int y, int radius, Matrix & mat, Matrix & mask);
	int gainPoints(int x, int y, int radius, const Matrix & mat, Matrix & mask);
	void placeMstPaths(std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists);
	int argMin(int * tab, int size);
	void findChessConnection(int * Routerfrom, int * RouterTo, std::vector<int *> & cables);
	bool routerOnBackbone(std::vector<int *> & routeurs);
	int cellsCoveredPercentage(Matrix & targetCells);
	bool isCyclical(Matrix & mat);
	int nbCellsCovered(Matrix & targetCells);
	void updateApproximateCost(int * newRouteurs, std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, int & approximateCost);
	void gaussianKernel(std::vector<float> & kernel, int W);
	void convolve(std::vector<float> & convolvedMat, Matrix & mat, std::vector<float> kernel, int w);
	void displayVector(const std::vector<int> &vect, int h, int w);
	void displayVector(const std::vector<float> &vect, int h, int w);
	int nbNewCoveredCells(int xRouter, int yRouter, Matrix & targetCells);
	void displayScore(Matrix & targetCells, int nbRouters);
	int nbNotTargetCellsAround(int xRouter, int yRouter, Matrix & targetCells);
};
