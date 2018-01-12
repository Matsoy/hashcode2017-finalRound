#pragma once
#include "Header.h"
#include "Matrix.h"
#include "IO.h"

/*
* Classe qui gere les algorithmes
*/
class Algo
{
private:
	std::string aMethod; // nom de l'algorithme utilise
	std::string aMapName; // nom de la carte sur laquelle est lance l'algorithme
	Matrix & aMap; /* matrice d'entiers selon 		-2 --> emetteur
													-1 --> vide
													0 --> mur
													1, --> cellule cible
													2, --> routeur
													3, --> routeur connecte
													4 --> cable
													*/
	Matrix & aMapSolution; // matrice d'entiers representant un fichier d'output lors de lancement de l'algorithme pour ameliorer l'output d'un fichier solution 
	int aRayonRouteurs; // rayon d'un routeur
	int aPrixCable; // prix d'un cable
	int aPrixRouteur; // prix d'un routeur
	int aBudget; // budget courant
	int aBudgetOriginal; // budget max
	int * aBackbone; // coordonnes [x, y] de l'emetteur
	int aNbCellsOriginal; // nombre de cellules cibles au depart
	int aBestScore; // meilleur score de la carte correspondante

public:
	/*
	* constructeur
	*/
	Algo(std::string method, std::string mapName, Matrix & map, Matrix & mapSolution,int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * backbone);

	/*
	* Acesseurs
	*/
	std::string getMethod() const; // type d'algo
	Matrix & getMap() const;
	int getRayonRouteurs() const;
	int getPrixCable() const;
	int getPrixRouteur() const;
	int getBudget() const; // budget courant
	int getBudgetOriginal() const; // budget max
	int * getBackbone() const; // coordonnes [x, y] de l'emetteur
	int getNbCellsOriginal() const; // nombre de cellules cibles au depart

	/*
	* Mutateur
	* Modifie aMapSolution
	*
	* @param mapSolution une instance de la classe Matrix
	*/
	void setMapSolution(Matrix & mapSolution);
	/*
	* Lance l'algorithme donne par le main
	*
	* @param bestScore le meilleur score correspond a la carte lancee. Permettant de voir apres l'algorithme si on a battu le meilleur score
	*/
	void run(int bestScore);
	/*
	* Effectue l'algorithme de Kruskal permettant de connaitre l'arbre minimal couvrant les routeurs.
	* On saura alors si l'ajout de ce nouveau routeur depassera ou non le budget
	*
	* @param newRouteurs tableau contenant les coordonnees du nouveau routeur a placer
	* @param routeurs tableau contenant les coordonnees des routeurs places
	* @param idx tableau des x
	* @param idy tableau des y
	* @param dist tableau d'entier des distance inter-routeurs
	* @param idy tableau des y
	* @param dist tableau d'entier des distance inter-routeurs
	* @param succ true si on ne depasse pas le budget
	*				false sinon
	* @param cost cout courant utilise pour placer les routeurs et les cables
	*/
	void kruskal(int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool & succ, int & cost) const;
	/*
	* Renvoie la distance entre routeur et newRouteur si un echiquier (diagonal, horizontal, vertical)
	*
	* @param routeur coordonnes d'un routeur deja place
	* @param newrouteur coordonnes d'un nouveau routeur
	* return la distance sur un echiquier
	*/
	int chessboardDist(const int * routeur, const int * newrouteur) const;
	/*
	* transforme la matrice en une matrice CSR (Compressed Sparse Row Matrix) i.e matrice symetrique des distances
	* ou data, row_ind et col_ind satisfont la relation
	* a[idx[k], idy[k]] = dists[k]
	* a[idy[k], idx[k]] = dists[k]
	*
	* @param dist tableau d'entier des distance inter-routeurs
	* @param idx tableau des x
	* @param idy tableau des y
	* @param dim dimension de la matrice csr, i.e. le nombre de routeurs
	*/
	void toCsrMatrix(Matrix & mat, const std::vector<int> & dists, const std::vector<int> & idx, const std::vector<int> & idy, const int dim) const;
	/*
	* renvoie un arbre couvrant minimal d'un graphe non oriente
	* Un arbre recouvrant minimal est un graphe constitue du sous-ensemble d'aretes qui relient
	* ensemble tous les noeuds connectes, tout en minimisant la somme totale des poids sur les aretes.
	* Ceci est calcule en utilisant l'algorithme de Kruskal.
	*
	* @param dist tableau d'entier des distance inter-routeurs
	* @param idx tableau des x
	* @param idy tableau des y
	* @param r nb de lignes de la matrice csr
	* @param c nb de colonnes de la matrice csr
	* @return la reference de cette matrice CSR de dimension r x c
	*/
	void toMinimumSpanningTree(const Matrix & csrMat, Matrix & mstMat) const;
	/*
	* trouve le sommet avec la valeur de cle minimale, a partir de l'ensemble des sommets non encore inclus dans le mst
	*
	* @param key vecteur des cles utilisees pour selectionner l'arete de poids minimum
	* @param mstSet vecteur de booleens pour representer un ensemble de sommets non encore inclus dans MST
	* @param dim la dimension de la matrice
	* @return le sommet avec la valeur de cle minimale
	*/
	int minKey(const std::vector<int> key, const std::vector<bool> mstSet, const int dim) const;
	/*
	* Algorithme
	* Prend aléatoirement une cellules non couverte et place un routeur dessus
	*/
	void random();
	void bigCase();
	/*
	* Algorithme
	* Effectue un flou gaussien sur la carte pour en deduire les cellules cibles les plus loins des murs, des cellules vides et des cellules dea couvertes
	*/
	void gaussianBlur();
	/*
	* Rempli le masque de la couverture d'un routeur place en (x, y).
	* 0 si il a au moins 1 mur,
	* 1 sinon.
	* Applique a postiori sur la carte. On pourra savoir si telle ou telle cellules est couverte par le routeur ou inaccessible a cause d'un mur
	*
	* @param x du routeur
	* @param y du routeur
	* @param mat la matrice de la carte
	* @param mask masque de la couverture du routeur
	*/
	void wirelessAccess(const int x, const int y, const Matrix & mat, Matrix & mask) const;
	int gainPoints(int x, int y, int radius, const Matrix & mat, Matrix & mask);
	/*
	* Modifie aMap en en y placant les cables et les routeurs pour pouvoir generer le fichier d'output
	*
	* @param routeurs vecteurs des routeurs
	* @param idx vecteur des x
	* @param idy vecteur des y
	* @param idy vecteur des distances
	*/
	void placeMstPaths(const std::vector<int *> & routeurs, const std::vector<int> & idx, const std::vector<int> & idy, const std::vector<int> & dists);
	/*
	* Modifie aMap en en y placant les cables et les routeurs pour pouvoir generer le fichier d'output
	*
	* @param routeurs vecteurs des routeurs
	* @param idx vecteur des x
	* @param idy vecteur des y
	* @param idy vecteur des distances
	*/
	void placeMstPaths_2(const std::vector<int *> & routeurs, const std::vector<int> & idx, const std::vector<int> & idy, const std::vector<int> & dists);
	/*
	* Renvoie l'indice de la valeur minimale dans un tableau d'entiers
	* En cas d'occurrences multiples des valeurs minimales, l'indice correspondant a la premiere occurrence est renvoyee.
	*
	* @param tab tableau d'entiers
	* @param size la taille du tableau
	* @return l'indice de la valeur minimale
	*/
	int argMin(const int * tab, const int size) const;
	/*
	* Ajoute dans le vecteur cables les cables reliant Routerfrom a RouterTo comme sur un echiquier (diagonal, horizontal et vertical)
	*
	* @param Routerfrom routeur de depart
	* @param Routerfrom routeur de destination
	* @param cables vecteur de pointeurs d'entiers contenant les positions des cables
	*/
	void findChessConnection(const int * Routerfrom, const int * RouterTo, std::vector<int *> & cables) const;
	/*
	* Ajoute dans vertices le point a l'intersection des 2 droites diagonales necessaires pour cabler Routerfrom a RouterTo
	*
	* @param Routerfrom routeur 1
	* @param Routerfrom routeur 2
	* @param vertices vecteur de pointeurs d'entiers contenant les sommets de l'arbre couvrant minimal
	*/
	void addIntersectInVertices(const int * Routerfrom, const int * RouterTo, std::vector<int *> & vertices) const;
	/*
	* Ajoute dans le vecteur cables les cables reliant Routerfrom a RouterTo en ne suivant que des diagonales
	*
	* @param Routerfrom routeur de depart
	* @param Routerfrom routeur de destination
	* @param cables vecteur de pointeurs d'entiers contenant les positions des cables
	*/
	void findDiagonalConnection(const int * Routerfrom, const int * RouterTo, std::vector<int *> & cables) const;
	/*
	* check si un routeur est sur le backbone
	*
	* @param routeurs vecteur des coordonnees des routeurs
	* @return true si un routeur est sur le backbone
	*			false sinon
	*/
	bool routerOnBackbone(const std::vector<int *> & routeurs) const;
	/*
	* renvoie le pourcentage de cellules cibles couvertes. i.e. les '.' du fichier d'input
	*
	* @param targetCells matrice avec les cellules cibles restantes
	* @return le pourcentage de cellules cibles couvertes
	*/
	int cellsCoveredPercentage(const Matrix & targetCells) const;
	/*
	* renvoie le nombre de cellules couvertes
	*
	* @param targetCells matrice avec les cellules cibles restantes
	* @return le nombre de cellules couvertes
	*/
	int nbCellsCovered(const Matrix & targetCells) const;
	/*
	* MaJ le budget approximatif qui sera majore, suite au placement d'un nouveau routeur, sans avoir besoin utiliser Kruskal (qui est couteux).
	* Cela permet de gagner du temps pour les premiers tours de l'algorithme.
	* Quand ce budge approximatif sera > au budget total, alors on fera Kruskal pour connaitre precisement le budget restant
	*
	* @see getApproximateCost
	* @param newRouteurs la position du nouveau routeur a essayer de placer
	* @param routeurs vecteur des routeurs deja places
	* @param idx vecteur des x
	* @param idy vecteur des y
	* @param idy vecteur des distances
	* @param approximateCost vecteur des distances
	* @return la distance entre le nouveau routeur et le routeur le plus proche
	*/
	void updateApproximateCost(const int * newRouteurs, const std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, int & approximateCost) const;
	/*
	* Renvoie la distance entre le nouveau routeur et le routeur le plus proche.
	*
	* @param newRouteurs la position du nouveau routeur a essayer de placer
	* @param routeurs vecteur des routeurs deja places
	* @param idx vecteur des x
	* @param idy vecteur des y
	* @param idy vecteur des distances
	* @return la distance entre le nouveau routeur et le routeur le plus proche
	*/
	int getApproximateCost(const int * newRouteurs, const std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists) const;
	/*
	* Renvoie le noyau du filtre gaussien, selon le parametre sigma genere aleatoirement entre 2 bornes
	*
	* @param kernel vecteur de float qui sera rempli par les valeurs du kernel gaussien
	* @param w largeur de la matrice du kernel
	*/
	void gaussianKernel(std::vector<float> & kernel, const int w) const;
	/*
	* Effectue une convolution: convolvedMat = mat**kernel
	* Ainsi, grace a convolvedMat, on pourra recuperer les cellules cibles les plus eloignees des murs, des cellules vides ou des cellules deja couvertes.
	* Cela est une maniere de recuperer des positions + ou - pertinentes pour y placer un routeur
	*
	* @param convolvedMat la matrice resultante de la convolution
	* @param mat la matrice a convoluer avec le kenel du flou gaussien, matrice avec les cellules cibles a 1 et le reste a -1
	* @param deadMat la matrice contenant les cellule qui n'auront aucun impact sur la convolution (on n'aura alors pas besoin de faire le produit de convolution qui est couteux)
	* @param kernel matrice du noyau du flou gaussien
	* @param w dimension du kernel
	*/
	void convolve(std::vector<float> & convolvedMat, const Matrix & mat, Matrix & deadMat, const std::vector<float> kernel, const int w) const;
	/*
	* Pour afficher une vecteur de int sous forme de matrice
	*
	* @param vect un vecteur d'entier
	* @param h hauteur de la matrice a afficher
	* @param w largeur de la matrice a afficher
	*/
	void displayVector(const std::vector<int> &vect, int h, int w) const;
	/*
	* Pour afficher une vecteur de float sous forme de matrice
	*
	* @param vect un vecteur de float
	* @param h hauteur de la matrice a afficher
	* @param w largeur de la matrice a afficher
	*/
	void displayVector(const std::vector<float> &vect, int h, int w) const;
	/*
	* renvoie le nombre de cellules couvertes apres l'ajout d'un routeur
	*
	* @param xRouter coordonnee x du nouveau routeur
	* @param yRouter coordonnee y du nouveau routeur
	* @param targetCells la matrice contenant l'information des cellules couvertes
	* @return le nombre de cellules couvertes
	*/
	int nbNewCoveredCells(const int xRouter, const int yRouter, const Matrix & targetCells) const;
	/*
	* Affiche le score realise
	*
	* @param targetCells la matrice contenant l'information des cellules couvertes
	* @param nbRouters le nombre de routeurs
	*/
	void displayScore(const Matrix & targetCells, const int nbRouters) const;
	/*
	* renvoie le nombre de cellules non couvertes dans les bordures du rayon d'un routeur
	*
	* @param xRouter coordonnee x du routeur
	* @param yRouter coordonnee y du routeur
	* @param targetCells la matrice contenant l'information des cellules couvertes
	* @return le nombre de cellules non couvertes dans les bordures du rayon d'un routeur
	*/
	int nbNotTargetCellsAround(const int xRouter, const int yRouter, const Matrix & targetCells) const;
	/*
	* Fonction principale qui ameliore un fichier solution et qui tente de mieux placer les cables pour degager du budget et si possible de rajouter des routeurs pour couvrair + de cellules
	*
	* @param routeurs vecteur des routeurs
	* @param pathBeginning debut du nom du fichier solution qui sera cree a dans la classe IO a la fin de cette fonction
	*/
	void centroid(std::vector<int *> & routeurs, IO & io, std::string pathBeginning);
	/*
	* Apres avoir recupere la position du barycentre du triangle forme par 3 points, on regarde si ajouter ce sommet permet d'utiliser moins de cables en utilisant Kruskal
	*
	* @param center le point barycentre
	* @param vertices vecteur des sommets
	* @param idx tableau des x
	* @param idy tableau des y
	* @param dist tableau d'entier des distance inter-routeurs
	* @param nbCablesBefore nombre de cables necessaire avant d'inserer le point center en tant que sommet dans l'arbre couvrant minimal
	* @param intermediateCsrMat contiendra la matric CSR si il l'ajout du point center ets valide
	* @return la difference entre le nombre de cables avant et apres ajout de ce point center. Si diff > 0, on validera l'ajout de ce nouveau point
	*/
	int centerIsBetter(int * center, std::vector<int *> & vertices, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, const int nbCablesBefore, Matrix & intermediateCsrMat);
	/*
	* renvoie le barycentre (point de Fermat) du triangle ABC si celui-ci est calculable (si tous les angles sont < 120° ),
	*			le centre de gravite sinon
	*
	* @param A le 1er point du triangle
	* @param B le 2eme point du triangle
	* @param C le 3eme point du triangle
	* @return le barycentre (point de Fermat) du triangle ABC si celui-ci est calculable,
	*			le centre de gravite sinon
	*/
	int * getCentroid(const int * A, const int * B, const int * C) const;
	/*
	* solution post-solution: on tente de rajouter un ou plusieurs routeurs apres avec reussi a degager du budget depuis un fichier d'output
	*
	* @param vertices verteurs des sommet de l'arbre courant minimal
	* @param idx tableau des x
	* @param idy tableau des y
	* @param dist tableau d'entier des distance inter-routeurs
	*/
	void addLastRouters(std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists);
	/*
	* Affiche le score realise apres cablage de la solution en faisant des barycentres
	*
	* @param targetCells la matrice contenant l'information des cellules couvertes
	* @param nbRouters le nombre de routeurs
	*/
	void displayScoreAfterCentroids(const Matrix & targetCells, const int nbRouters) const;
	/*
	* Soit le triangle ABC, on renvoie le point D qui composera le triangle equilateral ABD.
	* On choisira le D le plus eloigne de C
	*
	* @see getCentroid
	* @param A le point A du triangle ABC
	* @param B le point B du triangle ABC
	* @param C le point C du triangle ABC
	* @return un pointeur de float contenant les coordonnees (x, y) de ce point D
	*/
	float * get3rdPointEquilateral(const int * A, const int * B, const int * C) const;
	/*
	* Modifie aMap en en y placant les cables et les routeurs pour pouvoir generer le fichier d'output
	*
	* @param cables vecteurs des cables
	* @param vertices des sommets de l'arbre courant minimal
	*/
	void placeMstPaths_3(const std::vector<int *> & cables, const std::vector<int *> & vertices);
};


/*
* Accesseurs
*/
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

/*
* Renvoie la distance entre routeur et newRouteur si un echiquier (diagonal, horizontal, vertical)
*
* @param routeur coordonnes d'un routeur deja place
* @param newrouteur coordonnes d'un nouveau routeur
* return la distance sur un echiquier
*/
inline
int Algo::chessboardDist(const int * routeur, const int * newrouteur) const
{
	return std::max(abs(routeur[0] - newrouteur[0]), abs(routeur[1] - newrouteur[1]));
}