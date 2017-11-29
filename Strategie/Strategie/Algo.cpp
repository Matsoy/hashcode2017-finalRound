#include "Algo.h"
#include "Cell.h"

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
		catch (std::exception const& ) { }
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
void Algo::toMinimumSpanningTree(Matrix & mat, Matrix & mst)
{
	std::cout << "entree toMinimumSpanningTree" << std::endl;

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

	// remplissage de la matrice mst en ne gardant que les arcs de poids minimum
	for (int i = 1; i < dim; i++)
	{
		mst(parent[i], i) = mat(i, parent[i]);
	}

	std::cout << "sortie toMinimumSpanningTree" << std::endl;
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
void Algo::kruskal(Matrix & m, int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool ** succ, int ** cost)
{
	std::cout << "sortie kruskal" << std::endl;

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

	int intCost = mstMatWeight * aPrixCable + (routeurs.size() - 1) * aPrixRouteur;
	*cost = &intCost;

	bool boolSucc= intCost <= aBudgetOriginal;
	*succ = &boolSucc; // si on ne dépasse par le budget

	std::cout << "sortie kruskal" << std::endl;

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
	std::cout << "#####################entree wirelessAccess" << std::endl;

	// remplissage de mask
	for (int maskIndex= 0; maskIndex < mask.getCols() * mask.getRows(); maskIndex++)
	{
		mask(maskIndex) = INT_MIN;
	}
	
	// on parcour le perimetre de de gauche a droite et de haut en bas
	for (int dh = -radius; dh <= radius; dh++) // decalage en lignes
	{
		for (int dw = -radius; dw <= radius; dw++) // decalage en colonnes
		{
			std::cout << "dans for : dw = " << dw << ", dh = " << dh << std::endl;

			// on passe la cellule du routeur
			if (dh == 0 && dw == 0)
			{
				mask(dh + radius, dw + radius) = 1;
				continue;
			}

			std::cout << "apres if 1" << std::endl;

			// transforme les coordonnees
			int xt = x + dh;
			int yt = y + dw;

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << "xt = " << xt << std::endl;
			std::cout << "yt = " << yt << std::endl;
			std::cout << "dh = " << dh << std::endl;
			std::cout << "dw = " << dw << std::endl;
			std::cout << "mat.getRows() = " << mat.getRows() << std::endl;
			std::cout << "mat.getCols() = " << mat.getCols() << std::endl;

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

			std::cout << mask << std::endl;
		}
	}
	std::cout << mask << std::endl;
	std::cout << "#####################sortie wirelessAccess" << std::endl;

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
	int * cost = 0;
	bool * succ = false;

	// placement de l'emetteur en tant que routeur
	kruskal(aMap, aBackbone, routeurs, idx, idy, dists, &succ, &cost); // modifie cost, succ, routeurs, idx, idy et dists

	for (int currentRouterId = 0; currentRouterId < maxNumRouters; currentRouterId++)
	{
		// vecteur des coordonnees des cellules cibles n'ayant pas encore de routeur place dessus
		std::vector<int *> targetCellsCoords;

		for (int cellxy = 0; cellxy < targetCells.getRows() * targetCells.getCols(); cellxy++)
		{
				if (targetCells(cellxy) == 0) targetCellsCoords.push_back(targetCells.xy(cellxy));
		}


		if (targetCellsCoords.size()) // si il y au moins 1 position dispo pour le prochain routeur
		{
			// methode mst
			// on recupere une position random pour un prochain routeur
			std::random_device rd;     // seulement utilisé une fois pour initialiser le moteur (semence)
			std::mt19937 rng(rd());    // moteur à nombre aléatoire utilisé (Mersenne-Twister dans ce cas)
			std::uniform_int_distribution<int> uni(0, aMap.getRows() * aMap.getCols()); // garantie sans biais
			auto random_integer = uni(rng) % targetCellsCoords.size();

			int * xyNewRouter = targetCellsCoords[random_integer];
			
			// on garde en memoire l'etat de la case avant d'y mettre un routeur
			int tmp = aMap(xyNewRouter[0], xyNewRouter[1]);

			aMap(xyNewRouter[0], xyNewRouter[1]) = Cell::Router; // Cell::Router = 2

			// tentative de placement de ce routeur
			kruskal(aMap, xyNewRouter, routeurs, idx, idy, dists, &succ, &cost); // modifie cost, succ, routeurs, idx, idy et dists
			if (routeurs.size() % 10 == 0) std::cout << routeurs.size() << " routeurs " << std::endl;

			std::cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-Nouveau routeur en [" << xyNewRouter[0] << "," << xyNewRouter[1] << "]" << std::endl;

			if (succ) // && currentRouterId < 10
			{
				std::cout << "entree if (succ)" << std::endl;
				// recuperation du masque du perimetre du routeur
				Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

				wirelessAccess(xyNewRouter[0], xyNewRouter[1], aRayonRouteurs, aMap, mask);
			
				std::cout << "apres wirelessAccess" << std::endl;

				//targetCells[(xyNewRouter[0] - radius) : (xyNewRouter[0] + radius + 1), (xyNewRouter[1] - radius) : (xyNewRouter[0] + radius + 1)] |= mask.astype(np.bool)

				// on ne garde que les cellules ciblees dans le perimetre du routeur en fonction des murs
				for (int xTargetCell = xyNewRouter[0] - aRayonRouteurs; xTargetCell <= (xyNewRouter[0] + aRayonRouteurs + 1); xTargetCell++)
				{
					for (int yTargetCell = xyNewRouter[1] - aRayonRouteurs; (yTargetCell <= xyNewRouter[0] + aRayonRouteurs + 1); yTargetCell++)
					{
						//targetCells(xTargetCell, yTargetCell) |= mask.astype(np.bool)
					}
				}

				std::cout << "sortie if (succ)" << std::endl;
			}
			else // plus de budget
			{ 
				std::cout << "succ == false" << std::endl;
				// on enleve le dernier routeur
				aMap(xyNewRouter[0], xyNewRouter[1]) = tmp; // on retabli l'etat de la case
				// d = _place_mst_paths(d, routers, idx, idy, dists)

				std::cout << "Plus de budget !!" << std::endl;

			}
		}

		
	}
}

void Algo::run()
{
	if (aType == "theoreme_de_soyer") random();
}
