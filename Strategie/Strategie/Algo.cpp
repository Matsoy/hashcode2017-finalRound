#include "Algo.h"
#include "Cell.h"

Algo::Algo(std::string type, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * xyBackbone) : aType(type), aMap(map), aRayonRouteurs(rayonRouteurs), aPrixCable(prixCable), aPrixRouteur(prixRouteur), aBudget(budget), aBudgetOriginal(budgetOriginal), aBackbone(xyBackbone)
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
		for (int i = 0; i < abs(dx - dy); i++)
		{
			path(path.getRows() - 1, r[amin] + i) = 1; // passe a true
		}

	}
	else if (amin == 1)
	{
		for (int i = 0; i < abs(dx - dy); i++)
		{
			path(r[amin] + i, path.getCols() - 1) = 1; // passe a true
		}
	}

	// si la matrice a ete retournee, on la remet a l'endroit
	if (reversed) path.reverse();


	// on selectionne les cables
	for (int r = 0; r < path.getRows(); r++)
	{

		for (int c = 0; c < path.getCols(); c++)
		{

			// si true dans path, on place un cable a cette coordonnees
			if (path(r, c) == 1)
			{
				//std::cout << "cables. = " <<r << "c = "<<c << std::endl;

				cables.push_back(new int[2]{ r + xmin, c + ymin });
			}
		}
	}
}


/*
* transforme la matrice en une matrice CSR (Compressed Sparse Row Matrix) i.e matrice symetrique des distances
* ou data, row_ind et col_ind satisfont la relation
* a[idx[k], idy[k]] = dists[k]
*
* @param dist tableau d'entier des distance inter-routeurs
* @param idx tableau des x
* @param idy tableau des y
* @param dim dimension de la matrice csr, soit nombre_de_routeurs x nombre_de_routeurs
*/
void Algo::toCsrMatrix(Matrix & mat, std::vector<int> & dists, std::vector<int> & idx, std::vector<int> & idy, int dim)
{
	for (int k = 0; k < dists.size(); k++)
	{
		try
		{
			mat(idx[k], idy[k]) = dists[k];
		}
		catch (std::exception const&) {}
	}
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
		if (mstSet[v] == false && key[v] < min)
		{
			min = key[v];
			min_index = v;
		}

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
	for (int count = 0; count < dim - 1; count++)
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
		mst(parent[i], i) = mat(parent[i], i);
	}
}

void Algo::placeMstPaths(std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists)
{
	// calcul du mst
	Matrix csrMat(routeurs.size(), routeurs.size());
	toCsrMatrix(csrMat, dists, idx, idy, routeurs.size());

	// arbre couvrant minimal
	// i.e.  un graphe constitue du sous-ensemble d'aretes qui relient ensemble tous les noeuds connectes, tout en minimisant la somme totale des poids sur les aretes.
	Matrix mstMat(routeurs.size(), routeurs.size());
	toMinimumSpanningTree(csrMat, mstMat);

	// algo calcul distance entre les routeurs. Parcours de l'arbre couvrant minimal
	for (int r = 0; r < mstMat.getRows(); r++)
	{
		for (int c = 0; c < mstMat.getCols(); c++)
		{
			if (mstMat(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
			{
				std::vector<int *> cables;
				findChessConnection(routeurs[r], routeurs[c], cables);

				for (int *cable : cables) // pour chaque cable
				{
					if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1]) // si le cable est sur l'emetteur
						continue; // on passe

					if (aMap(cable[0], cable[1]) == Cell::Router) // si le cable est sur un routeur
						aMap(cable[0], cable[1]) = Cell::ConnectedRouter; // on passe la valeur de la coordonnee de 2 a 3 pour indiquer que le routeur est connecte au backbone
					else
						aMap(cable[0], cable[1]) = Cell::Cable; // sinon on place tout simplement le cable
				}
			}
		}
	}
	for (int * rout : routeurs)
	{
		if (rout[0] == aBackbone[0] && rout[1] == aBackbone[1]) // si le cable est sur l'emetteur
			continue; // on passe

		aMap(rout[0], rout[1]) = Cell::ConnectedRouter; // on passe la valeur de la coordonnee de 2 a 3 pour indiquer que le routeur est connecte au backbone
	}
}

/*
* @param m la matrice de base avec les murs, les cellules cibles et les cellules vides
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

	Matrix csrMat(routeurs.size(), routeurs.size());
	toCsrMatrix(csrMat, dists, idx, idy, routeurs.size()); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
														   // [0 x x 1]
														   // [0 0 x 7]
														   // [0 0 0 2]

														   // arbre couvrant minimal
	Matrix mstMat(routeurs.size(), routeurs.size());
	toMinimumSpanningTree(csrMat, mstMat);

	// poids de l'arbre couvrant minimum. i.e le nombre d'aretes
	int mstMatWeight = mstMat.sum();

	cost = mstMatWeight * aPrixCable + (routeurs.size() - 1) * aPrixRouteur; // routeur - 1 car on interprete le backbone comme un routeur dans l'algorithme

	succ = cost <= aBudgetOriginal; // si on ne depasse par le budget
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
	for (int maskIndex = 0; maskIndex < mask.getCols() * mask.getRows(); maskIndex++)
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
				continue;

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
			for (int wallsIndex = 0; wallsIndex < walls.getRows() * walls.getCols(); wallsIndex++)
			{
				walls(wallsIndex) = rect(wallsIndex) == Cell::Wall ? 1 : 0; // si mur ==> 1, 0 sinon
			}

			if (walls.sum()) mask(dh + radius, dw + radius) = 0; // si il a au moins 1 mur
			else mask(dh + radius, dw + radius) = 1; // si aucun mur
		}
	}

}

/*
* defini aleatoirement la position des routeurs
*/
void Algo::random()
{
	std::cout << "Algo -> " << aType << std::endl;

	// nb max de routeurs possibles
	int maxNumRouters = aBudget / aPrixRouteur;
	std::cout << "Nombre max de routeurs possibles -> " << maxNumRouters << std::endl;

	// matrice avec les cellules cibles a 0 et le reste a 1
	Matrix targetCells(aMap.getRows(), aMap.getCols());

	for (int k = 0; k < aMap.getRows() * aMap.getCols(); k++)
	{
		targetCells(k) = aMap(k) == Cell::Wireless ? 0 : 1;
	}

	std::vector<int *> routeurs;
	std::vector<int> idx;
	std::vector<int> idy;
	std::vector<int> dists;
	int cost = 0;
	bool succ = false;
	int testtest[2] = { 0, 1 };

	// placement de l'emetteur en tant que routeur
	kruskal(aMap, aBackbone, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists

	for (int currentRouterId = 0; currentRouterId < maxNumRouters; currentRouterId++)
	{
		//std::cout << currentRouterId +1 << " routeurs" << std::endl;
		// vecteur des coordonnees des cellules cibles n'ayant pas encore de routeur place dessus
		std::vector<int *> targetCellsCoords;

		for (int cellxy = 0; cellxy < targetCells.getRows() * targetCells.getCols(); cellxy++)
		{
			if (targetCells(cellxy) == 0) targetCellsCoords.push_back(targetCells.xy(cellxy));
		}

		// si il y au moins 1 position dispo pour le prochain routeur
		if (targetCellsCoords.size() == 0)
		{
			std::cout << "il n'y a plus de place pour placer des routeurs" << std::endl;
			return; // on sort alors de l'algo
		}

		// ########### methode mst ###########
		// ###################################

		// on recupere une position random pour un prochain routeur
		std::random_device rd;     // seulement utilise une fois pour initialiser le moteur (semence)
		std::mt19937 rng(rd());    // moteur a nombre aleatoire utilise (Mersenne-Twister dans ce cas)
		std::uniform_int_distribution<int> uni(0, aMap.getRows() * aMap.getCols()); // garantie sans biais

		auto random_integer = uni(rng) % targetCellsCoords.size();
		int * xyNewRouter = targetCellsCoords[random_integer];

		// on garde en memoire l'etat de la case avant d'y mettre un routeur au ca ou celui-ci ne conviendrait pas
		int safeguardingInfo = aMap(xyNewRouter[0], xyNewRouter[1]);

		aMap(xyNewRouter[0], xyNewRouter[1]) = Cell::Router; // Cell::Router = 2. On positionne le routeur sur la carte

															 // tentative de placement de ce routeur
		kruskal(aMap, xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists
		if (routeurs.size() % 5 == 0)
		{
			std::cout << routeurs.size() << " routeurs " << std::endl;
		}
		//std::cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Nouveau routeur en [" << xyNewRouter[0] << "," << xyNewRouter[1] << "]" << std::endl;

		// si encore du budget et moins de 10 routeurs en attente d'etre relies
		if (succ  && currentRouterId < 100) //  && currentRouterId < 10
		{
			//std::cout << "entree if (succ)" << std::endl;
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(xyNewRouter[0], xyNewRouter[1], aRayonRouteurs, aMap, mask);

			int indexMask = 0;
			// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
			for (int xTargetCell = xyNewRouter[0] - aRayonRouteurs; xTargetCell <= (xyNewRouter[0] + aRayonRouteurs); xTargetCell++)
			{
				for (int yTargetCell = xyNewRouter[1] - aRayonRouteurs; yTargetCell <= (xyNewRouter[1] + aRayonRouteurs); yTargetCell++)
				{
					if (mask(indexMask) == 1) targetCells(xTargetCell, yTargetCell) = 1;
					indexMask++;
				}
			}

			//std::cout << "sortie if (succ)" << std::endl;
		}
		else // plus de budget
		{
			//std::cout << "succ == false" << std::endl;
			// on enleve le dernier routeur
			aMap(xyNewRouter[0], xyNewRouter[1]) = safeguardingInfo; // on retabli l'etat de la case

			placeMstPaths(routeurs, idx, idy, dists);

			std::cout << "Plus de budget !!" << std::endl;

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
	if (aType == "random") random();

	// on remet le backbone
	aMap(aBackbone[0], aBackbone[1]) = Cell::Backbone; // -2

}
