#include "Algo.h"



Algo::Algo(std::string type, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * xyBackbone) : aType(type), aMap(map), aRayonRouteurs(rayonRouteurs),  aPrixCable(prixCable),  aPrixRouteur(prixRouteur), aBudget(budget),  aBudgetOriginal(budgetOriginal), aBackbone(xyBackbone)
{
}

// Accesseurs
std::string Algo::getType()
{
	return aType;
}

Matrix & Algo::getMap()
{
	return aMap;
}

int Algo::getRayonRouteurs()
{
	return aRayonRouteurs;
}

int Algo::getPrixCable()
{
	return aPrixCable;
}

int Algo::getPrixRouteur()
{
	return aPrixRouteur;
}
int Algo::getBudget()
{
	return aBudget;
}

int Algo::getBudgetOriginal()
{
	return aBudgetOriginal;
}
int * Algo::getBackbone()
{
	return aBackbone;
}

int Algo::chessboardDist(int*  routeur, int * newrouteur)
{
	int x1 = routeur[0];
	int y1 = routeur[1];
	int x2 = newrouteur[0];
	int y2 = newrouteur[1];

	return std::max(abs(x1 - x2), abs(y1 - y2));
}

int Algo::quasiEuclideanDist(int*  routeur, int * newrouteur)
{
	int x1 = routeur[0];
	int y1 = routeur[1];
	int x2 = newrouteur[0];
	int y2 = newrouteur[1];

	if (abs(x1 - x2) > abs(y1 - y2)) return abs(x1 - x2) + (sqrt(2) - 1) * abs(y1 - y2);
	return (sqrt(2) - 1) * abs(x1 - x2) + abs(y1 - y2);
}

/*
* renvoi une matrice CSR (Compressed Sparse Row Matrix) i.e matrice symetrique des distances
* ou data, row_ind et col_ind satisfont la relation
* a[idx[k], idy[k]] = dists[k]
*
* @param dist tableau d'entier des distance inter-routeurs
* @param idx tableau des x
* @param idy tableau des y
* @param dim dimension de la matrice csr, soit nombre_de_routeurs x nombre_de_routeurs 
*/
Matrix & Algo::csrMatrix(std::vector<int> & dists, std::vector<int> & idx, std::vector<int> & idy, int dim)
{
	Matrix mat(dim, dim);
	for (int k = 0; k < dists.size(); k++)
	{
		try
		{
			mat(idx[k], idy[k]) = dists[k];
		}
		catch (std::exception const& ) { }
	}

	return mat;
}

/*
* trouve le sommet avec la valeur de cle minimale, a partir de l'ensemble des sommets non encore inclus dans le mst
*
* @param keyvecteur des cles utilisees pour selectionner l'arete de poids minimum
* @param mstSet vecteur de booleens pour representer un ensemble de sommets non encore inclus dans MST
* @param dim la dimension de la matrice
* @return le sommet avec la valeur de cle minimale
*/
int Algo::minKey(std::vector<int> key, std::vector<bool> mstSet, int dim)
{
	// initialise la valeur minimum
	int min = INT_MAX, min_index;

	for (int v = 0; v < dim; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}


/*
* renvoie un arbre couvrant minimal d'un graphe non oriente
* Un arbre recouvrant minimal est un graphe constitue du sous-ensemble d'aretes qui relient 
* ensemble tous les nœuds connectes, tout en minimisant la somme totale des poids sur les aretes. 
* Ceci est calcule en utilisant l'algorithme de Kruskal.
*
* @param dist tableau d'entier des distance inter-routeurs
* @param idx tableau des x
* @param idy tableau des y
* @param r nb de lignes de la matrice csr
* @param c nb de colonnes de la matrice csr
* @return la reference de cette matrice CSR de dimension r x c
*/
Matrix & Algo::minimumSpanningTree(Matrix & mat)
{
	// dimension de la matrice
	int dim = mat.getRows(); 

	std::vector<int> parent(dim); // vecteur pour stocker l'arbre couvrant minimal
	std::vector<int> key(dim);   // cles utilisees pour selectionner l'arete de poids minimum
	std::vector<bool> mstSet(dim);  // pour representer un ensemble de sommets non encore inclus dans MST

	// initialisation de toutes les cles a +infini
	for (int keyI= 0; keyI < key.size(); keyI++)
	{
		key[keyI] = INT_MAX;
		mstSet[keyI] = false;
	}


	// on inclue le premier 1er sommet dans le mst
	key[0] = 0;     // cle a 0 pour que ce sommet soit choisi comme premier sommet
	parent[0] = -1; // Le 1er noeud est toujours la racine du mst

	// The mst aura "dim" sommets
	for (int count = 0; count < dim - 1; count++)
	{
		// on choisie le sommet de cle minimum de l'ensemble des sommets par encore inclus dans le mst
		int u = minKey(key, mstSet, dim);

		// Ajoute le sommet selectionne au mst
		mstSet[u] = true;

		// maj la valeur de cle et l'index parent des sommets adjacents du sommet sélectionne
		// on ne considere que les sommets qui ne sont pas encore inclus dans le mst
		for (int v = 0; v < dim; v++)
			if (mat(u, v) // mat[u][v] != 0 uniquement pour les sommets adjacents de m
				&& mstSet[v] == false // mstSet[v] == false pour les sommets non encore inclus dans MST
				&& mat(u, v) < key[v]) // maj la cle ssi mat[u][v] < key[v]
			{
				parent[v] = u;
				key[v] = mat(u, v);
			}
	}

	// matrice mst
	Matrix ret(dim, dim);

	// remplissage de la matrice mst en ne gardant que les arcs de poids minimum
	for (int i = 1; i < dim; i++)
	{
		ret(parent[i], i) = mat(i, parent[i]);
	}

	return ret;
}

/*
* @param m la matrice de base avec les murs, les cellules cibles et les cellules vides
* @param newRouteurs tableau contenant les coordonnees du routeur a placer
* @param newRouteurs tableau contenant les coordonnees des routeurs places
* @param idx tableau des x
* @param idy tableau des y
* @param dist tableau d'entier des distance inter-routeurs
*/
void Algo::kruskal(Matrix & m, int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists)
{
	int new_id = routeurs.size();

	int cpt = 0;

	// algo calcul distance entre les routeurs
	for (int * r : routeurs)
	{
		int dist = chessboardDist(r, newRouteurs);
		if (dist > 0)
		{
			idx.push_back(cpt); // ex: idx = [0, 1, 2]
			idy.push_back(new_id); // ex: idy = [3, 3, 3]
			dists.push_back(dist); // ex: dists = [1, 7, 4]
		}

		cpt++;
	}

	routeurs.push_back(newRouteurs);

	Matrix csrMat = csrMatrix(dists, idx, idy, routeurs.size()); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
																		  // [0 x x 1]
																		  // [0 0 x 7]
																		  // [0 0 0 2]
	// arbre couvrant minimal
	Matrix mstMat = minimumSpanningTree(csrMat);

	// poids de l'arbre couvrant minimum. i.e le nombre d'aretes
	int mstMatWeight = mstMat.sum();

	int cost = mstMatWeight * aPrixCable + (routeurs.size() - 1) * aPrixRouteur;
	bool succ = cost <= aBudgetOriginal; // si on ne dépasse par le budget
}

/*
* defini aleatoirement la position des routeurs
*/
void Algo::random()
{
	std::cout << "aType = " << aType << std::endl;

	// nb max de routeurs possibles
	int max_num_routers = aBudget / aPrixRouteur;
	std::cout << "max_num_routers = " << max_num_routers << std::endl;

	int _budget = aBudget;
	int R = aRayonRouteurs;

	std::vector<int *> routeurs;
	std::vector<int> idx;
	std::vector<int> idy;
	std::vector<int> dists;
	int cost = 0;
	bool succ = false;

	// placement de l'emetteur en tant que routeur
	kruskal(aMap, aBackbone, routeurs, idx, idy, dists); // modifie cost, succ, routeurs, idx, idy et dists
}

void Algo::run()
{
	if (aType == "theoreme_de_soyer") random();
}
