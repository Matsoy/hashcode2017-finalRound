#pragma once
#include "Header.h"
#include "Matrix.h"
#include "IO.h"

class Algo
{
private:
	std::string aMethod;
	std::string aMapName;
	Matrix & aMap;
	Matrix & aMapSolution;
	int aRayonRouteurs;
	int aPrixCable;
	int aPrixRouteur;
	int aBudget; // budget courant
	int aBudgetOriginal; // budget max
	int * aBackbone; // coordonnes [x, y] de l'emetteur
	int aNbCellsOriginal; // nombre de cellules cibles au depart
	int aBestScore; // meilleur score de la carte correspondante

public:
	Algo(std::string method, std::string mapName, Matrix & map, Matrix & mapSolution,int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * backbone);

	// accesseurs
	std::string getMethod() const; // type d'algo
	Matrix & getMap() const;
	int getRayonRouteurs() const;
	int getPrixCable() const;
	int getPrixRouteur() const;
	int getBudget() const; // budget courant
	int getBudgetOriginal() const; // budget max
	int * getBackbone() const; // coordonnes [x, y] de l'emetteur
	int getNbCellsOriginal() const; // nombre de cellules cibles au depart

	// mutateurs
	void setMapSolution(Matrix & mapSolution);

	void run(int bestScore);
	void kruskal(int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool & succ, int & cost) const;
	int chessboardDist(const int * routeur, const int * newrouteur) const;
	void toCsrMatrix(Matrix & mat, const std::vector<int> & dists, const std::vector<int> & idx, const std::vector<int> & idy, const int dim) const;
	void toMinimumSpanningTree(const Matrix & csrMat, Matrix & mstMat) const;
	int minKey(const std::vector<int> key, const std::vector<bool> mstSet, const int dim) const;
	void random();
	void bigCase();
	void gaussianBlur();
	void wirelessAccess(const int x, const int y, const Matrix & mat, Matrix & mask) const;
	int gainPoints(int x, int y, int radius, const Matrix & mat, Matrix & mask);
	void placeMstPaths(const std::vector<int *> & routeurs, const std::vector<int> & idx, const std::vector<int> & idy, const std::vector<int> & dists);
	void placeMstPaths_2(const std::vector<int *> & routeurs, const std::vector<int> & idx, const std::vector<int> & idy, const std::vector<int> & dists);
	int argMin(const int * tab, const int size) const;
	void findChessConnection(const int * Routerfrom, const int * RouterTo, std::vector<int *> & cables) const;
	bool routerOnBackbone(const std::vector<int *> & routeurs) const;
	int cellsCoveredPercentage(const Matrix & targetCells) const;
	bool isCyclical(Matrix & mat);
	int nbCellsCovered(const Matrix & targetCells) const;
	void updateApproximateCost(const int * newRouteurs, const std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, int & approximateCost) const;
	int getApproximateCost(const int * newRouteurs, const std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists) const;
	void gaussianKernel(std::vector<float> & kernel, const int w) const;
	void convolve(std::vector<float> & convolvedMat, const Matrix & mat, Matrix & deadMat, const std::vector<float> kernel, const int w) const;
	void displayVector(const std::vector<int> &vect, int h, int w) const;
	void displayVector(const std::vector<float> &vect, int h, int w) const;
	int nbNewCoveredCells(const int xRouter, const int yRouter, const Matrix & targetCells) const;
	void displayScore(const Matrix & targetCells, const int nbRouters) const;
	int nbNotTargetCellsAround(const int xRouter, const int yRouter, const Matrix & targetCells) const;
	void centroid(std::vector<int *> & routeurs, IO & io, std::string pathBeginning);
	int centerIsBetter(int * center, std::vector<int *> & vertices, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, const int nbCablesBefore, Matrix & intermediateCsrMat);
	int * getCentroid(const int * pt1, const int * pt2, const int * pt3) const;
	void addLastRouters(std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists);
	void displayScoreAfterCentroids(const Matrix & targetCells, const int nbRouters) const;
	float * get3rdPointEquilateral(const int * A, const int * B, const int * C) const;
};


// Accesseurs
inline
Matrix & Algo::getMap() const
{
	return aMap;
}

inline
int * Algo::getBackbone() const
{
	return aBackbone;
}

inline
int Algo::getNbCellsOriginal() const
{
	return aNbCellsOriginal;
}

inline
int Algo::getRayonRouteurs() const
{
	return aRayonRouteurs;
}

inline
int Algo::getPrixCable() const
{
	return aPrixCable;
}

inline
int Algo::getPrixRouteur() const
{
	return aPrixRouteur;
}

inline
int Algo::getBudget() const
{
	return aBudget;
}

inline
int Algo::getBudgetOriginal() const
{
	return aBudgetOriginal;
}

inline
std::string Algo::getMethod() const
{
	return aMethod;
}

inline
int Algo::chessboardDist(const int * routeur, const int * newrouteur) const
{
	return std::max(abs(routeur[0] - newrouteur[0]), abs(routeur[1] - newrouteur[1]));
}