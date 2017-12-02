#include "Algo.h"
#include "Cell.h"

Algo::Algo(std::string method, std::string mapName, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * xyBackbone) : aMethod(method), aMapName(mapName), aMap(map), aRayonRouteurs(rayonRouteurs), aPrixCable(prixCable), aPrixRouteur(prixRouteur), aBudget(budget), aBudgetOriginal(budgetOriginal), aBackbone(xyBackbone)
{
}

// Accesseurs
std::string Algo::getMethod()
{
	return aMethod;
}

std::string Algo::getMapName()
{
	return aMapName;
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

int Algo::getNbCellsOriginal()
{
	return aNbCellsOriginal;
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

	if (abs(x1 - x2) > abs(y1 - y2))
	{
		return abs(x1 - x2) + (sqrt(2) - 1) * abs(y1 - y2);
	}
	return (sqrt(2) - 1) * abs(x1 - x2) + abs(y1 - y2);
}

bool Algo::isCyclical(Matrix & mat)
{
	return true;
}

/*
* Renvoie l'indice la valeur minimale
* En cas d'occurrences multiples des valeurs minimales, l'indice correspondant a la premiere occurrence est renvoyee.
*
* @param tab tableau d'entiers
* @param size la taille du tableau
* @return argmin
*/
int Algo::argMin(int * tab, int size)
{
	int argmin = 0; // indice de l'argmin
	int argminValue = INT_MAX; // valeur de l'argmin

	for (int i = 0; i < size; i++)
	{
		if (tab[i] < argminValue)
		{
			argmin = i;
			argminValue = tab[i];
		}
	}

	return argmin;
}

/*
* renvoie le pourcentage de cellules cibles couvertes. i.e. les '.' du fichier d'input
*
* @param targetCells matrice avec les cellules cibles restante
* @return le pourcentage de cellules cibles couvertes
*/
int Algo::cellsCoveredPercentage(Matrix & targetCells)
{
	// nombre de cellules non couvertes
	int nbCellsNotCovered = 0;
	int condition = targetCells.getRows() * targetCells.getCols();
	for (int k = 0; k < condition; k++)
	{
		if (targetCells(k) == 0)
		{
			nbCellsNotCovered++;
		}
	}

	return ((aNbCellsOriginal - nbCellsNotCovered) * 100) / aNbCellsOriginal;
}


/*
* renvoie le nombre de cellules couvertes
*
* @param targetCells matrice avec les cellules cibles restante
* @return le nombre de cellules couvertes
*/
int Algo::nbCellsCovered(Matrix & targetCells)
{
	// nombre de cellules non couvertes
	int nbCellsNotCovered = 0;
	int condition = targetCells.getRows() * targetCells.getCols();
	for (int k = 0; k < condition; k++)
	{
		if (targetCells(k) == 0)
		{
			nbCellsNotCovered++;
		}
	}

	return aNbCellsOriginal - nbCellsNotCovered;
}



void Algo::findChessConnection(int * Routerfrom, int * RouterTo, std::vector<int *> & cables)
{
	//std::cout << "_________________________________dans findChessConnection" << std::endl;

	// vecteur des cables entre les 2 routeurs aux positions Routerfrom et RouterTo
	int dx = abs(Routerfrom[0] - RouterTo[0]) + 1;
	int dy = abs(Routerfrom[1] - RouterTo[1]) + 1;

	int xmin = std::min(Routerfrom[0], RouterTo[0]);
	int ymin = std::min(Routerfrom[1], RouterTo[1]);

	// matrice de booleens (0 -> false, 1 -> true)
	Matrix path(dx, dy);

	path(Routerfrom[0] - xmin, Routerfrom[1] - ymin) = 1;
	path(RouterTo[0] - xmin, RouterTo[1] - ymin) = 1;

	int r[2] = { dx, dy };
	int amin = argMin(r, 2);


	// true si la matrice path a ete renversee, false sinon
	bool reversed = false;


	if (!path(0, 0))
	{
		// on renverse la matrice dans le sens haut / bas
		path.reverse();

		// on indique que la matrice a ete renversee
		reversed = true;
	}

	// on defini les diagonales
	for (int i = 0; i < r[amin]; i++)
	{
		path(i, i) = 1;
	}


	// on defini les elements droits restants
	if (amin == 0)
	{
		int condition = abs(dx - dy);
		for (int i = 0; i < condition; i++)
		{
			path(path.getRows() - 1, r[amin] + i) = 1; // passe a true
		}

	}
	else if (amin == 1)
	{
		int condition = abs(dx - dy);
		for (int i = 0; i < condition; i++)
		{
			path(r[amin] + i, path.getCols() - 1) = 1; // passe a true
		}
	}

	// si la matrice a ete retournee, on la remet a l'endroit
	if (reversed)
	{
		path.reverse();
	}

	int rows = path.getRows();
	// on selectionne les cables
	for (int r = 0; r < rows; r++)
	{
		int cols = path.getCols();
		for (int c = 0; c < path.getCols(); c++)
		{
			// si true dans path, on place un cable a cette coordonnees
			if (path(r, c) == 1)
			{
				cables.push_back(new int[2]{ r + xmin, c + ymin });
			}
		}
	}
}


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
void Algo::toCsrMatrix(Matrix & mat, std::vector<int> & dists, std::vector<int> & idx, std::vector<int> & idy, int dim)
{
	int condition = dists.size();
	for (int k = 0; k < condition; k++)
	{
		try
		{
			mat(idx[k], idy[k]) = dists[k];
		}
		catch (std::exception const&) {}
		try
		{
			mat(idy[k], idx[k]) = dists[k];
		}
		catch (std::exception const&) {}
	}
}

/*
* check si un routeur est sur le backbone
*
* @param routeurs vecteur des coordonnees des routeurs
* @return true si un routeur est sur le backbone
*			false sinon
*/
bool Algo::routerOnBackbone(std::vector<int *> & routeurs)
{
	bool backbonePassed = false;
	for (int * rout : routeurs)
	{
		if (!backbonePassed) // on passe le 1er routeur pcq il correspond au backbone
		{
			backbonePassed = true;
			continue;
		}

		if (rout[0] == aBackbone[0] && rout[1] == aBackbone[1])
		{
			return true;
		}
	}

	return false;
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
	int min = INT_MAX;
	int min_index = 0;

	for (int v = 0; v < dim; v++)
	{
		if (mstSet[v] == false && key[v] < min)
		{
			min = key[v];
			min_index = v;
		}
	}
		

	//std::cout << "min_index -> " << min_index << std::endl;
	return min_index;
}


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
void Algo::toMinimumSpanningTree(Matrix & mat, Matrix & mst)
{
	//std::cout << "                           entree toMinimumSpanningTree" << std::endl;


	// dimension de la matrice
	int dim = mat.getRows();

	std::vector<int> parent(dim); // vecteur pour stocker l'arbre couvrant minimal
	std::vector<int> key(dim);   // cles utilisees pour selectionner l'arete de poids minimum
	std::vector<bool> mstSet(dim);  // pour representer un ensemble de sommets non encore inclus dans MST

									// initialisation de toutes les cles a +infini et des ensembles de sommets a false
	for (int i = 0; i < dim; i++)
	{
		key[i] = INT_MAX;
		mstSet[i] = false;
	}



	// on inclue le premier 1er sommet dans le mst
	key[0] = 0;     // cle a 0 pour que ce sommet soit choisi comme premier sommet
	parent[0] = -1; // Le 1er noeud est toujours la racine du mst

					// The mst aura "dim" sommets
	int dimMinusOne = dim - 1;
	for (int count = 0; count < dimMinusOne; count++)
	{
		// on choisie le sommet de cle minimum de l'ensemble des sommets par encore inclus dans le mst
		int u = minKey(key, mstSet, dim);
		//std::cout << "+ petit sommet = " << u << std::endl;

		// Ajoute le sommet selectionne au mst
		mstSet[u] = true;

		// maj la valeur de cle et l'index parent des sommets adjacents du sommet selectionne
		// on ne considere que les sommets qui ne sont pas encore inclus dans le mst
		for (int v = 0; v < dim; v++)
		{
			//std::cout << "on regarde mat(" << u << "," << v << ")" << std::endl;
			if (mat(u, v) // mat[u][v] != 0 uniquement pour les sommets adjacents de m
				&& mstSet[v] == false // mstSet[v] == false pour les sommets non encore inclus dans MST
				&& mat(u, v) < key[v]) // maj la cle ssi mat[u][v] < key[v]
			{
				parent[v] = u;
				key[v] = mat(u, v);
				//std::cout << "on maj la cle pour v = " << v << std::endl;
			}
		}
	}

	// remplissage de la matrice mst en ne gardant que les arcs de poids minimum
	for (int i = 1; i < dim; i++)
	{
		//std::cout << "mat(" <<parent[i] << "," << i <<") = " << mat(parent[i], i) << std::endl;

		mst(parent[i], i) = mat(parent[i], i);
	}
}


void Algo::placeMstPaths(std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists)
{
	//std::cout << "________________dans placeMstPaths________________" << std::endl;
	// calcul du mst
	Matrix csrMat(routeurs.size(), routeurs.size());
	toCsrMatrix(csrMat, dists, idx, idy, routeurs.size());

	//std::cout << "csrMat" << std::endl;
	//std::cout << csrMat << std::endl;

	// arbre couvrant minimal
	// i.e.  un graphe constitue du sous-ensemble d'aretes qui relient ensemble tous les noeuds connectes, tout en minimisant la somme totale des poids sur les aretes.
	Matrix mstMat(routeurs.size(), routeurs.size());
	toMinimumSpanningTree(csrMat, mstMat);

	//std::cout << "mstMat" << std::endl;
	//std::cout << mstMat << std::endl;
	int rows = mstMat.getRows();
	int cols = mstMat.getCols();
	// algo calcul distance entre les routeurs. Parcours de l'arbre couvrant minimal
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (mstMat(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
			{
				std::vector<int *> cables;
				findChessConnection(routeurs[r], routeurs[c], cables);

				for (int *cable : cables) // pour chaque cable
				{
					// si le cable est sur l'emetteur
					if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
					{
						continue;
					}

					// si le cable est sur un routeur
					if (aMap(cable[0], cable[1]) == Cell::Router)
					{
						aMap(cable[0], cable[1]) = Cell::ConnectedRouter; // on passe la valeur de la coordonnee de 2 a 3 pour indiquer que le routeur est connecte au backbone
					}
					else
					{
						aMap(cable[0], cable[1]) = Cell::Cable; // sinon on place tout simplement le cable
					}
				}
			}
		}
	}

	bool backbonePassed = false;
	for (int * rout : routeurs)
	{
		if (!backbonePassed) // on passe le 1er routeur pcq il correspond au backbone
		{
			backbonePassed = true;
			continue;
		}

		aMap(rout[0], rout[1]) = Cell::ConnectedRouter; // on passe la valeur de la coordonnee de 2 a 3 pour indiquer que le routeur est connecte au backbone
	}

}
void Algo::updateApproximateCost(int * newRouteurs, std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, int & approximateCost)
{
	int distMin = INT_MAX;
	int new_id = routeurs.size();
	int cpt = 0;

	// on récupère la distance avec le routeur le plus proche
	for (int * rout : routeurs)
	{
		int dist = chessboardDist(rout, newRouteurs);
		if (dist > 0)
		{
			idx.push_back(cpt);
			idy.push_back(new_id);
			dists.push_back(dist);
		}
		if (dist < distMin)
		{
			distMin = dist;
		}
		cpt++;
	}

	approximateCost += (distMin * aPrixCable + aPrixRouteur);
}


/*
* @param m la matrice de la carte
* @param newRouteurs tableau contenant les coordonnees du routeur a placer
* @param newRouteurs tableau contenant les coordonnees des routeurs places
* @param idx tableau des x
* @param idy tableau des y
* @param dist tableau d'entier des distance inter-routeurs
* @param idy tableau des y
* @param dist tableau d'entier des distance inter-routeurs
* @param succ true si on ne depasse pas le budget
*				false sinon
* @param cost cout courant utilise pour placer les routeurs et les cables
*/
void Algo::kruskal(Matrix & m, int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool & succ, int & cost)
{
	//std::cout << "sortie kruskal" << std::endl;

	int new_id = routeurs.size();

	int cpt = 0;

	// algo calcul distance entre les routeurs
	for (int * rout : routeurs)
	{
		int dist = chessboardDist(rout, newRouteurs);
		if (dist > 0)
		{
			idx.push_back(cpt); // ex: idx = [0, 1, 2]
			idy.push_back(new_id); // ex: idy = [3, 3, 3]
			dists.push_back(dist); // ex: dists = [1, 7, 4]
		}
		cpt++;
	}

	routeurs.push_back(newRouteurs);

	Matrix csrMat(routeurs.size(), routeurs.size());
	toCsrMatrix(csrMat, dists, idx, idy, routeurs.size()); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
														   // [0 x x 1]
														   // [0 0 x 7]
														   // [0 0 0 2]

														   //std::cout << "csrMat" << std::endl;
														   //std::cout << csrMat << std::endl;

														   // arbre couvrant minimal
	Matrix mstMat(routeurs.size(), routeurs.size());
	toMinimumSpanningTree(csrMat, mstMat);

	// poids de l'arbre couvrant minimum. i.e le nombre d'aretes
	int mstMatWeight = mstMat.sum();

	//std::cout << "mstMat" << std::endl;
	//std::cout << mstMat << std::endl;

	cost = mstMatWeight * aPrixCable + (routeurs.size() - 1) * aPrixRouteur; // routeur - 1 car on interprete le backbone comme un routeur dans l'algorithme

	succ = cost <= aBudgetOriginal; // si on ne depasse par le budget

									//std::cout << mstMatWeight << " * " << aPrixCable << " + " << " (" << routeurs.size() << " - " << 1 << " ) * " << aPrixRouteur << " = " << cost << "  sur " << aBudgetOriginal << std::endl;
									//std::cout << succ << std::endl;
}
/*
* @param x du routeur
* @param y du routeur
* @param radius rayon du routeur
* @param mat la matrice de la carte
* @return la matrice masque
*/
void Algo::wirelessAccess(int x, int y, int radius, Matrix & mat, Matrix & mask)
{
	// remplissage de mask
	int condition = mask.getCols() * mask.getRows();
	for (int maskIndex = 0; maskIndex < condition; maskIndex++)
	{
		mask(maskIndex) = -1; // par defaut, on place des bordures partout
	}

	// on parcourt le perimetre de de gauche a droite et de haut en bas
	for (int dh = -radius; dh <= radius; dh++) // decalage en lignes
	{
		for (int dw = -radius; dw <= radius; dw++) // decalage en colonnes
		{
			// on passe la cellule du routeur
			if (dh == 0 && dw == 0)
			{
				mask(dh + radius, dw + radius) = 1;
				continue;
			}

			// transforme les coordonnees
			int xt = x + dh;
			int yt = y + dw;

			// on check les bordures
			if (xt >= mat.getRows() || xt < 0 || yt >= mat.getCols() || yt < 0)
			{
				continue;
			}

			// si pas une cellule cible
			if (mat(xt, yt) != Cell::Wireless)
			{
				// on met les autres champs a 0
				mask(dh + radius, dw + radius) = 0;

				continue;
			}

			// on construit la + petite rectangle contenant le routeur et la cellule pour pouvoir savoir si la portee du routeur est coupee par un mur
			// coordonnees des 4 sommets du rectangle
			int fromX = std::min(x, xt);
			int toX = std::max(x, xt);
			int fromY = std::min(y, yt);
			int toY = std::max(y, yt);

			Matrix rect(toX - fromX + 1, toY - fromY + 1);

			// prochain index de rect a remplir
			int nextRectIndex = 0;
			// remplissage de rect avec les valeurs de mat correspondantes
			for (int r = fromX; r <= toX; r++)
			{
				for (int c = fromY; c <= toY; c++)
				{
					rect(nextRectIndex) = mat(r, c);
					nextRectIndex++;
				}
			}

			// matrice des murs
			Matrix walls(rect.getRows(), rect.getCols());
			// remplissage de walls
			int condition2 = walls.getCols() * walls.getRows();
			for (int wallsIndex = 0; wallsIndex < condition2; wallsIndex++)
			{
				walls(wallsIndex) = rect(wallsIndex) == Cell::Wall ? 1 : 0; // si mur ==> 1, 0 sinon
			}

			if (walls.sum())
			{
				mask(dh + radius, dw + radius) = 0; // si il a au moins 1 mur
			}
			else
			{
				mask(dh + radius, dw + radius) = 1; // si aucun mur
			}
		}
	}

}

int Algo::gainPoints(int x, int y, int radius, const Matrix & mat, Matrix & mask)
{
	int count = 0;
	// remplissage de mask
	int condition = mask.getCols() * mask.getRows();
	for (int maskIndex = 0; maskIndex < condition; maskIndex++)
	{
		mask(maskIndex) = -1; // par defaut, on place des bordures partout
	}

	// on parcourt le perimetre de de gauche a droite et de haut en bas
	for (int dh = -radius; dh <= radius; dh++) // decalage en lignes
	{
		for (int dw = -radius; dw <= radius; dw++) // decalage en colonnes
		{
			// on passe la cellule du routeur
			if (dh == 0 && dw == 0)
			{
				mask(dh + radius, dw + radius) = 1;
				continue;
			}

			// transforme les coordonnees
			int xt = x + dh;
			int yt = y + dw;

			// on check les bordures
			if (xt >= mat.getRows() || xt < 0 || yt >= mat.getCols() || yt < 0)
			{
				continue;
			}

			// si pas une cellule cible
			if (mat(xt, yt) != Cell::Wireless)
			{
				// on met les autres champs a 0
				mask(dh + radius, dw + radius) = 0;

				continue;
			}

			// on construit la + petite rectangle contenant le routeur et la cellule pour pouvoir savoir si la portee du routeur est coupee par un mur
			// coordonnees des 4 sommets du rectangle
			int fromX = std::min(x, xt);
			int toX = std::max(x, xt);
			int fromY = std::min(y, yt);
			int toY = std::max(y, yt);

			Matrix rect(toX - fromX + 1, toY - fromY + 1);

			// prochain index de rect a remplir
			int nextRectIndex = 0;
			// remplissage de rect avec les valeurs de mat correspondantes
			for (int r = fromX; r <= toX; r++)
			{
				for (int c = fromY; c <= toY; c++)
				{
					rect(nextRectIndex) = mat(r, c);
					nextRectIndex++;
				}
			}

			// matrice des murs
			Matrix walls(rect.getRows(), rect.getCols());
			// remplissage de walls
			int condition2 = walls.getCols()*walls.getRows();
			for (int wallsIndex = 0; wallsIndex < condition2; wallsIndex++)
			{
				walls(wallsIndex) = rect(wallsIndex) == Cell::Wall ? 1 : 0; // si mur ==> 1, 0 sinon
			}

			if (walls.sum()) {
				mask(dh + radius, dw + radius) = 0; // si il a au moins 1 mur
			}
			else {
				mask(dh + radius, dw + radius) = 1; // si aucun mur
			}
		}
	}
	for (int maskIndex = 0; maskIndex < condition; maskIndex++)
	{
		if (mask(maskIndex) == 1) {
			count += 1;
		}
	}
	return count;

}

/*
* defini aleatoirement la position des routeurs
*/
void Algo::random()
{
	// nb max de routeurs possibles
	int maxNumRouters = aBudget / aPrixRouteur;
	std::cout << "  Budget / prix d'un routeur = \t" << maxNumRouters << " routeurs\n" << std::endl;
	std::cout << "  --------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "  Routeurs\t\t|\t\t% budget utilise\t\t|\t\t% cellules couvertes" << std::endl;
	std::cout << "  --------------------------------------------------------------------------------------------------------------------" << std::endl;


	// matrice avec les cellules cibles a 0 et le reste a 1
	Matrix targetCells(aMap.getRows(), aMap.getCols());
	int condition = aMap.getRows() * aMap.getCols();
	for (int k = 0; k < aMap.getRows() * aMap.getCols(); k++)
	{
		if (aMap(k) == Cell::Wireless)
		{
			targetCells(k) = 0;
			aNbCellsOriginal++;
		}
		else
		{
			targetCells(k) = 1;
		}
	}

	std::vector<int *> routeurs;
	std::vector<int> idx;
	std::vector<int> idy;
	std::vector<int> dists;
	int cost = 0; // cout reel calcul d'apres le mst
	int approximateCost = 0; // cout approximatif calcul dans le mst, en reliant simplement le routeur vers le routeur courant vers son plus proche
	bool succ = true;
	bool useKruskal = false; // vrai que le cout approximatif > budjet

	// on place le backbone dans le vecteur de routeur. 
	// Bien que n'etant pas un routeur avec pouvant couvrir des cellules, il represente un sommet dans la construction d'un mst avec Kruskal
	routeurs.push_back(aBackbone);

	for (int currentRouterId = 0; currentRouterId < maxNumRouters; currentRouterId++)
	{
		//std::cout << currentRouterId +1 << " routeurs" << std::endl;
		// vecteur des coordonnees des cellules cibles n'ayant pas encore de routeur place dessus
		std::vector<int *> targetCellsCoords;
		int condition2 = targetCells.getRows() * targetCells.getCols();
		for (int cellxy = 0; cellxy < condition2; cellxy++)
		{
			if (targetCells(cellxy) == 0)
			{
				targetCellsCoords.push_back(targetCells.xy(cellxy));
			}
		}

		// si il n'y a pas de position dispo pour le prochain routeur
		if (targetCellsCoords.size() == 0)
		{
			//std::cout << "il n'y a plus de place pour placer de routeurs" << std::endl;

			// on tire les cables
			placeMstPaths(routeurs, idx, idy, dists);

			std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << (useKruskal ? cost : approximateCost) << " / " << aBudgetOriginal << " = " << ((useKruskal ? cost : approximateCost) * 100) / aBudgetOriginal << "%" << (useKruskal ? "\t" : "") << "\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
			std::cout << '\r';

			return; // et on sort de l'algo
		}

		// on recupere une position random pour un prochain routeur
		std::random_device rd;     // seulement utilise une fois pour initialiser le moteur (semence)
		std::mt19937 rng(rd());    // moteur a nombre aleatoire utilise (Mersenne-Twister dans ce cas)
		std::uniform_int_distribution<int> uni(0, condition); // garantie sans biais

		auto random_integer = uni(rng) % targetCellsCoords.size();
		int * xyNewRouter = targetCellsCoords[random_integer];
			
		// on garde en memoire l'etat de la case avant d'y mettre un routeur au ca ou celui-ci ne conviendrait pas
		int safeguardingInfo = aMap(xyNewRouter[0], xyNewRouter[1]);

		aMap(xyNewRouter[0], xyNewRouter[1]) = Cell::Router; // Cell::Router = 2. On positionne le routeur sur la carte

		// tentative de placement de ce routeur
		// on sauvegarde l'etat des variables au cas ou on ne pourrait pas placer ce routeur
		std::vector<int> idxTmp = idx;
		std::vector<int> idyTmp = idy;
		std::vector<int> distsTmp = dists;
		int costTmp = useKruskal ? cost : approximateCost;

		// pour eviter de faire le mst, on calcule approximativement le cout de place de ce routeur en le cablant a son plus proche voisin
		if (!useKruskal)
		{
			updateApproximateCost(xyNewRouter, routeurs, idx, idy, dists, approximateCost);

			// si le cout approximatif indique que le cout est depasse, on utilisera kruskal pour connaitre le cout reel
			if (approximateCost > aBudgetOriginal)
			{
				useKruskal = true;
				kruskal(aMap, xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists

			}
			else
			{
				routeurs.push_back(xyNewRouter);
			}
		}
		else {
			kruskal(aMap, xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists
		}

		//std::cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Nouveau routeur en [" << xyNewRouter[0] << "," << xyNewRouter[1] << "]" << std::endl;

		// si encore du budget
		if (succ)
		{
			//std::cout << "entree if (succ)" << std::endl;
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(xyNewRouter[0], xyNewRouter[1], aRayonRouteurs, aMap, mask);

			int indexMask = 0;
			// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
			int condition3 = xyNewRouter[0] + aRayonRouteurs;
			for (int xTargetCell = xyNewRouter[0] - aRayonRouteurs; xTargetCell <= xyNewRouter[0] + aRayonRouteurs; xTargetCell++)
			{
				int condition4 = xyNewRouter[1] + aRayonRouteurs;
				for (int yTargetCell = xyNewRouter[1] - aRayonRouteurs; yTargetCell <= condition4; yTargetCell++)
				{
					if (mask(indexMask) == 1)
					{
						targetCells(xTargetCell, yTargetCell) = 1;
					}
					indexMask++;
				}
			}


			if (((routeurs.size() - 1) % 1) == 0)
			{
				std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal?"":"env. ") << (useKruskal ? cost : approximateCost ) << " / " << aBudgetOriginal << " = " << ((useKruskal ? cost : approximateCost) * 100) / aBudgetOriginal << "%" << (useKruskal ? "\t" : "") << "\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
				std::cout << '\r';

			}
		}
		else // plus de budget, on enleve le routeur en trop
		{
			//std::cout << "\n\n\nPlus de budget !" << std::endl;
			//std::cout << "succ == false" << std::endl;

			// on retabli l'etat de la case dans la carte
			aMap(xyNewRouter[0], xyNewRouter[1]) = safeguardingInfo;

			// on supprime le routeur de trop du vecteur
			routeurs.erase(routeurs.begin() + routeurs.size() - 1);

			// on reduit la taille du vecteur au nombre d'elements dans le vecteur
			routeurs.shrink_to_fit();

			// on tire les cables
			placeMstPaths(routeurs, idxTmp, idyTmp, distsTmp);
			std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << costTmp << " / " << aBudgetOriginal << " = " << (costTmp * 100) / aBudgetOriginal << "%" << "\t\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
			std::cout << '\r';

			return;

		}
	}
	// #################################################################################
	// peut etre a enlever pr les grosses map
	placeMstPaths(routeurs, idx, idy, dists);
	// #################################################################################
}

void Algo::bigCase()
{
	// nb max de routeurs possibles
	int maxNumRouters = aBudget / aPrixRouteur;
	std::cout << "  Budget / prix d'un routeur = \t" << maxNumRouters << " routeurs\n" << std::endl;
	std::cout << "  --------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "  Routeurs\t\t|\t\t% budget utilise\t\t|\t\t% cellules couvertes" << std::endl;
	std::cout << "  --------------------------------------------------------------------------------------------------------------------" << std::endl;


	// matrice avec les cellules cibles a 0 et le reste a 1
	Matrix targetCells(aMap.getRows(), aMap.getCols());
	int condition = aMap.getRows() * aMap.getCols();
	for (int k = 0; k < condition; k++)
	{
		if (aMap(k) == Cell::Wireless)
		{
			targetCells(k) = 0;
			aNbCellsOriginal++;
		}
		else targetCells(k) = 1;
	}

	std::vector<int *> routeurs;
	std::vector<int> idx;
	std::vector<int> idy;
	std::vector<int> dists;
	int cost = 0; // cout reel calcul d'apres le mst
	int approximateCost = 0; // cout approximatif calcul dans le mst, en reliant simplement le routeur vers le routeur courant vers son plus proche
	bool succ = true;
	bool useKruskal = false; // vrai que le cout approximatif > budjet

							 // on place le backbone dans le vecteur de routeur. 
							 // Bien que n'etant pas un routeur avec pouvant couvrir des cellules, il represente un sommet dans la construction d'un mst avec Kruskal
	routeurs.push_back(aBackbone);

	for (int currentRouterId = 0; currentRouterId < maxNumRouters; currentRouterId++)
	{
		//std::cout << currentRouterId +1 << " routeurs" << std::endl;
		// vecteur des coordonnees des cellules cibles n'ayant pas encore de routeur place dessus
		std::vector<int *> targetCellsCoords;
		int bestMove = 0, otherMove = 0;
		int * theChosenOne = 0;
		int condition2 = targetCells.getRows() * targetCells.getCols();
		for (int cellxy = 0; cellxy < targetCells.getRows() * targetCells.getCols(); cellxy++)
		{
			if (targetCells(cellxy) == 0) {
				targetCellsCoords.push_back(targetCells.xy(cellxy));
				/*Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);
				otherMove = gainPoints(targetCellsCoords[cellxy][0], targetCellsCoords[cellxy][1], aRayonRouteurs, aMap, mask);
				if (otherMove >= bestMove) {
					theChosenOne = targetCellsCoords[cellxy];
				}*/
			}
		}
		int sizeTestCells = targetCellsCoords.size();
		for (int cellxy = 0; cellxy < sizeTestCells; cellxy++)
		{
			//std::cout << cellxy << "\n";
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);
			otherMove = gainPoints(targetCellsCoords[cellxy][0], targetCellsCoords[cellxy][1], aRayonRouteurs, aMap, mask);
			if (otherMove >= bestMove) {
				theChosenOne = targetCellsCoords[cellxy];
			}
		}

		// si il n'y a pas de position dispo pour le prochain routeur
		if (targetCellsCoords.size() == 0)
		{
			//std::cout << "il n'y a plus de place pour placer de routeurs" << std::endl;

			// on tire les cables
			placeMstPaths(routeurs, idx, idy, dists);

			std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << (useKruskal ? cost : approximateCost) << " / " << aBudgetOriginal << " = " << ((useKruskal ? cost : approximateCost) * 100) / aBudgetOriginal << "%" << (useKruskal ? "\t" : "") << "\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
			std::cout << '\r';

			return; // et on sort de l'algo
		}

		// on garde en memoire l'etat de la case avant d'y mettre un routeur au ca ou celui-ci ne conviendrait pas
		int safeguardingInfo = aMap(theChosenOne[0], theChosenOne[1]);

		aMap(theChosenOne[0], theChosenOne[1]) = Cell::Router; // Cell::Router = 2. On positionne le routeur sur la carte

															   // tentative de placement de ce routeur
															   // on sauvegarde l'etat des variables au cas ou on ne pourrait pas placer ce routeur
		std::vector<int> idxTmp = idx;
		std::vector<int> idyTmp = idy;
		std::vector<int> distsTmp = dists;
		int costTmp = useKruskal ? cost : approximateCost;

		// pour eviter de faire le mst, on calcule approximativement le cout de place de ce routeur en le cablant a son plus proche voisin
		if (!useKruskal)
		{
			updateApproximateCost(theChosenOne, routeurs, idx, idy, dists, approximateCost);

			// si le cout approximatif indique que le cout est depasse, on utilisera kruskal pour connaitre le cout reel
			if (approximateCost > aBudgetOriginal)
			{
				useKruskal = true;
				kruskal(aMap, theChosenOne, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists

			}
			else
			{
				routeurs.push_back(theChosenOne);
			}
		}
		else {
			kruskal(aMap, theChosenOne, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists
		}

																				 //std::cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Nouveau routeur en [" << xyNewRouter[0] << "," << xyNewRouter[1] << "]" << std::endl;

																				 // si encore du budget
		if (succ)
		{
			//std::cout << "entree if (succ)" << std::endl;
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(theChosenOne[0], theChosenOne[1], aRayonRouteurs, aMap, mask);

			int indexMask = 0;
			int condition3 = theChosenOne[0] + aRayonRouteurs;
			// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
			for (int xTargetCell = theChosenOne[0] - aRayonRouteurs; xTargetCell <= condition3; xTargetCell++)
			{
				int condition4 = theChosenOne[1] + aRayonRouteurs;
				for (int yTargetCell = theChosenOne[1] - aRayonRouteurs; yTargetCell <= condition4; yTargetCell++)
				{
					if (mask(indexMask) == 1)
					{
						targetCells(xTargetCell, yTargetCell) = 1;
					}
					indexMask++;
				}
			}


			if (((routeurs.size() - 1) % 1) == 0)
			{
				std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << (useKruskal ? cost : approximateCost) << " / " << aBudgetOriginal << " = " << ((useKruskal ? cost : approximateCost) * 100) / aBudgetOriginal << "%" << (useKruskal ? "\t" : "") << "\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
				std::cout << '\r';

			}
		}
		else // plus de budget, on enleve le routeur en trop
		{
			//std::cout << "\n\n\nPlus de budget !" << std::endl;
			//std::cout << "succ == false" << std::endl;

			// on retabli l'etat de la case dans la carte
			aMap(theChosenOne[0], theChosenOne[1]) = safeguardingInfo;

			// on supprime le routeur de trop du vecteur
			routeurs.erase(routeurs.begin() + routeurs.size() - 1);

			// on reduit la taille du vecteur au nombre d'elements dans le vecteur
			routeurs.shrink_to_fit();

			// on tire les cables
			placeMstPaths(routeurs, idxTmp, idyTmp, distsTmp);
			std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << costTmp << " / " << aBudgetOriginal << " = " << (costTmp * 100) / aBudgetOriginal << "%" << "\t\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
			std::cout << '\r';

			return;

		}
	}
	// #################################################################################
	// peut etre a enlever pr les grosses map
	placeMstPaths(routeurs, idx, idy, dists);
	// #################################################################################
}

void Algo::run()
{
	if (aMethod == "random")
	{
		std::cout << "  Algo =\t\t\t" << aMethod << std::endl;
		bigCase();
	}
}
