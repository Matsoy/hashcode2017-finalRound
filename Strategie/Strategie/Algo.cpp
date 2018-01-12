#include "Algo.h"

Algo::Algo(std::string method, std::string mapName, Matrix & map, Matrix & mapSolution, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * xyBackbone) : aMethod(method), aMapName(mapName), aMap(map), aMapSolution(mapSolution), aRayonRouteurs(rayonRouteurs), aPrixCable(prixCable), aPrixRouteur(prixRouteur), aBudget(budget), aBudgetOriginal(budgetOriginal), aBackbone(xyBackbone)
{
}

// mutateurs
void Algo::setMapSolution(Matrix & mapSolution)
{
	aMapSolution = mapSolution;
}

//Pour afficher une vecteur de int sous forme de matrice
void Algo::displayVector(const std::vector<int> &vect, const int h, const int w) const
{
	std::string out;

	int m = 0, n = 0, cpt = 0;

	for (int j = 0; j <(h * w); j++)
	{
		m = cpt / w;
		n = cpt % w;

		// si 1er elem
		if (n == 0) out += "[ ";

		// si dernier elem
		if ((n == w - 1))
		{
			out += std::to_string(vect[j]) + " ]";
			std::cout << out << std::endl;
			out.clear();
		}
		else out += std::to_string(vect[j]) + ", ";

		cpt++;
	}
}

//Pour afficher une vecteur de float sous forme de matrice
void Algo::displayVector(const std::vector<float> &vect, const int h, const int w) const
{
	std::string out;

	int m = 0, n = 0, cpt = 0;

	for (int j = 0; j <(h * w); j++)
	{
		m = cpt / w;
		n = cpt % w;

		// si 1er elem
		if (n == 0) out += "[ ";

		// si dernier elem
		if ((n == w - 1))
		{
			out += std::to_string(vect[j]) + " ]";
			std::cout << out << std::endl;
			out.clear();
		}
		else out += std::to_string(vect[j]) + ", ";

		cpt++;
	}
}



// affiche le score realise
void Algo::displayScoreAfterCentroids(const Matrix & targetCells, const int nbRouters) const
{
	// nombre de cellules non couvertes
	int nbCellsNotCovered = 0;
	for (int k = 0; k < targetCells.getRows() * targetCells.getCols(); k++)
	{
		if (targetCells(k) == 0) nbCellsNotCovered++;
	}

	// nombre de cables
	int nbCables = nbRouters;
	for (int i = 0; i < aMap.getRows() * aMap.getCols(); i++)
	{
		if (aMap(i) == Cell::Cable) nbCables++;
	}

	int score = 1000 * (aNbCellsOriginal - nbCellsNotCovered) + (aBudgetOriginal - nbRouters * aPrixRouteur - nbCables * aPrixCable);
	std::cout << "Nb de cellules couvertes = " << (aNbCellsOriginal - nbCellsNotCovered) << std::endl;
	std::cout << "nbRouters = " << nbRouters << std::endl;
	std::cout << "score = " << score << std::endl;

	std::cout << "\n\n  Score =\t\t" << score << std::endl;

	if (aMapName == "charleston_road" && score > 21962821)
	{
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "\t\t\t|\t\tMEILLEUR SCORE\t\t\t|" << std::endl;
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	if (aMapName == "rue_de_londres" && score > 58356020)
	{
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "\t\t\t|\t\tMEILLEUR SCORE\t\t\t|" << std::endl;
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
	if (aMapName == "opera" && score > 174527096)
	{
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "\t\t\t|\t\tMEILLEUR SCORE\t\t\t|" << std::endl;
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
}

void Algo::displayScore(const Matrix & targetCells, const int nbRouters) const
{
	// nombre de cellules non couvertes
	int nbCellsNotCovered = 0;
	for (int k = 0; k < targetCells.getRows() * targetCells.getCols(); k++)
	{
		if (targetCells(k) == 0) nbCellsNotCovered++;
	}

	// nombre de cables
	int nbCables = nbRouters;
	for (int i = 0; i < aMap.getRows() * aMap.getCols(); i++)
	{
		if (aMap(i) == Cell::Cable) nbCables++;
	}

	int score = 1000 * (aNbCellsOriginal - nbCellsNotCovered) + (aBudgetOriginal - nbRouters * aPrixRouteur - nbCables * aPrixCable);

	std::cout << "\n\n  Score =\t\t" << score << std::endl;
	if (aBestScore < score) {
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "\t\t\t|\t\tMEILLEUR SCORE\t\t\t|" << std::endl;
		for (int i = 0; i < 5; i++) std::cout << "\t\t\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
}



int Algo::nbNewCoveredCells(const int xRouter, const int yRouter, const Matrix & targetCells) const
{
	// recuperation du masque du perimetre du routeur
	Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);
	wirelessAccess(xRouter, yRouter, aMap, mask);

	int nbNewCoveredCells = 0;
	int indexMask = 0;

	const int xFrom = xRouter - aRayonRouteurs;
	const int xTo = xRouter + aRayonRouteurs;
	const int yFrom = yRouter - aRayonRouteurs;
	const int yTo= yRouter + aRayonRouteurs;
	// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
	for (int xTargetCell = xFrom; xTargetCell <= xTo; xTargetCell++)
	{
		for (int yTargetCell = yFrom; yTargetCell <= yTo; yTargetCell++)
		{
			// si la cellule est une nouvelle cellule couverte
			if (mask(indexMask) == 1 && targetCells(xTargetCell, yTargetCell) == 0)	nbNewCoveredCells++;
			indexMask++;
		}
	}

	return nbNewCoveredCells;
}


int Algo::nbNotTargetCellsAround(const int xRouter, const int yRouter, const Matrix & targetCells) const
{
	int nbNotTargetCellsAround = 0;

	const int xTopBorder = xRouter - aRayonRouteurs - 1;
	const int xBottomBorder = xRouter + aRayonRouteurs + 1;
	const int yLeftBorder = yRouter - aRayonRouteurs - 1;
	const int yRightBorder = yRouter + aRayonRouteurs + 1;


	// parcour de la bordure gauche et droite du perimetre
	for (int xTargetCell = xTopBorder; xTargetCell <= xBottomBorder; xTargetCell++)
	{
		if (targetCells(xTargetCell, yLeftBorder)) nbNotTargetCellsAround++;
		if (targetCells(xTargetCell, yRightBorder)) nbNotTargetCellsAround++;
	}


	// parcour de la bordure haut et bas du perimetre
	for (int yTargetCell = yLeftBorder; yTargetCell <= yRightBorder; yTargetCell++)
	{
		if (targetCells(xTopBorder, yTargetCell)) nbNotTargetCellsAround++;
		if (targetCells(xBottomBorder, yTargetCell)) nbNotTargetCellsAround++;
	}

	return nbNotTargetCellsAround;
}

void Algo::convolve(std::vector<float> & convolvedMat, const Matrix & mat, Matrix & deadMat, const std::vector<float> kernel, const int w) const
{
	const int kernelHalfDim = w / 2;
	const int kernelSize = w * w;
	const int height = mat.getRows();
	const int width = mat.getCols();

	// pour chaque case de la matrice mat
	for (int rowM = 0; rowM < height; rowM++)
	{
		for (int colM = 0; colM < width; colM++)
		{
			// si la cellule est une cellule morte. i.e une cellule morte = une cellule qui n'aura aucun impact sur la convolution
			// on n'aura alors pas besoin de faire le produit de convolution qui est couteux
			if (deadMat(rowM, colM) == 1)
			{
				convolvedMat[rowM * width + colM] = -kernelSize;
			}
			else
			{
				float sumOfProducts = 0;
				// pour chaque case du kernel gaussien
				for (int rowK = kernelHalfDim; rowK <= kernelHalfDim; rowK++)
				{
					for (int colK = -kernelHalfDim; colK <= kernelHalfDim; colK++)
					{
						int x = rowM + rowK;
						int y = colM + colK;

						// si m(x, y) ne depasse pas les bordures
						if (x >= 0 && y >= 0 && x < height && y < width)
						{
							sumOfProducts += mat(x, y) * kernel[(rowK + kernelHalfDim) * w + (colK + kernelHalfDim)];
						}
					}
				}
				convolvedMat[rowM * width + colM] = sumOfProducts;
				// si dans le perimetre de la cellule il n'y a que des cellules a -1 dans targetCellsForConvolution
				if (sumOfProducts == -kernelSize)
				{
					deadMat(rowM, colM) = 1;
				}
			}
		}
	}

}

/*
* Renvoie l'indice la valeur minimale
* En cas d'occurrences multiples des valeurs minimales, l'indice correspondant a la premiere occurrence est renvoyee.
*
* @param tab tableau d'entiers
* @param size la taille du tableau
* @return argmin
*/
int Algo::argMin(const int * tab, const int size) const
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
int Algo::cellsCoveredPercentage(const Matrix & targetCells) const
{
	// nombre de cellules non couvertes
	int nbCellsNotCovered = 0;
	const int size = targetCells.getRows() * targetCells.getCols();

	for (int k = 0; k < size; k++)
	{
		if (targetCells(k) == 0)
		{
			nbCellsNotCovered++;
		}
	}

	return ((aNbCellsOriginal - nbCellsNotCovered) * 100) / aNbCellsOriginal;
}


void Algo::gaussianKernel(std::vector<float> & kernel, const int w) const
{

	// on prend un sigma aleatoire
	std::random_device rd;     // seulement utilise une fois pour initialiser le moteur (semence)
	std::mt19937 rng(rd());    // moteur a nombre aleatoire utilise (Mersenne-Twister dans ce cas)
	std::uniform_real_distribution<float> uni(0, 0.1); // garantie sans biais

	const float sigma = uni(rng);
	//float sigma = 0.01;
	std::cout << "sigma = " << sigma << std::endl;


	//float sigma = 10; // + sigma est grand, + le flou gaussien est fort, + les valeur du kernel gaussien seront proches 
	const float mean = w / 2;
	const float PI = 3.14159265;
	float sum = 0.0; // pour accumuler les valeurs du noyau
	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < w; ++y)
		{
			kernel[x * w + y] = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0))) / (2 * PI * sigma * sigma);

			// on accumule les valeurs du noyau
			sum += kernel[x * w + y];
		}
	}

	// on normalise le noyau
	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < w; ++y)
		{
			kernel[x * w + y] /= sum;
		}
	}
}

/*
* renvoie le nombre de cellules couvertes
*
* @param targetCells matrice avec les cellules cibles restante
* @return le nombre de cellules couvertes
*/
int Algo::nbCellsCovered(const Matrix & targetCells) const
{
	// nombre de cellules non couvertes
	int nbCellsNotCovered = 0;
	const int size = targetCells.getRows() * targetCells.getCols();

	for (int k = 0; k < size; k++)
	{
		if (targetCells(k) == 0)
		{
			nbCellsNotCovered++;
		}
	}

	return aNbCellsOriginal - nbCellsNotCovered;
}

void Algo::addIntersectInVertices(const int * Routerfrom, const int * RouterTo, std::vector<int *> & vertices) const
{
	int distX = abs(Routerfrom[0] - RouterTo[0]);
	int distY = abs(Routerfrom[1] - RouterTo[1]);

	int * from1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
	int * to1 = new int[2]{ RouterTo[0], RouterTo[1] };

	// ##########################################
	// on passe par le haut ou par le bas
	// ##########################################
	if (distX + 1 < distY)
	{
		// du routeur le + a gauche vers le routeur le + a droite
		if (Routerfrom[1] > RouterTo[1])
		{
			from1 = new int[2]{ RouterTo[0], RouterTo[1] };
			to1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
		}
		// par le haut
		if (((from1[0] - (distY / 2)) >= 0) && ((to1[0] - (distY / 2)) >= 0))
		{
			int currentX = from1[0] - 1;
			int currentY = from1[1] + 1;

			// on monte
			while (true)
			{
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1)
				{
					vertices.push_back(new int[2]{ currentX , currentY });
					return;
				}
				currentX--;
				currentY++;
			}
		}

		// par le bas
		else
		{
			int currentX = from1[0] + 1;
			int currentY = from1[1] + 1;

			// on descend
			while (true)
			{
				// si on doit changer de sens
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1)
				{
					vertices.push_back(new int[2]{ currentX , currentY });
					return;
				}
				currentX++;
				currentY++;
			}
		}
	}

	// ##########################################
	//on passe par la gauche ou par la droite
	// ##########################################
	else if (distX > distY + 1)
	{
		// du routeur le + en haut vers le routeur le + en bas
		if (Routerfrom[0] > RouterTo[0])
		{
			from1 = new int[2]{ RouterTo[0], RouterTo[1] };
			to1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
		}

		// par la gauche
		if (((from1[1] - (distX / 2)) >= 0) && ((to1[1] - (distX / 2)) >= 0))
		{
			int currentX = from1[0] + 1;
			int currentY = from1[1] - 1;

			// on va a gauche
			while (true)
			{
				// si on doit changer de sens
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1)
				{
					vertices.push_back(new int[2]{ currentX , currentY });
					return;
				}
				currentX++;
				currentY--;
			}
		}

		// par la droite
		else
		{
			int currentX = from1[0] + 1;
			int currentY = from1[1] + 1;

			// on va a droite
			while (true)
			{
				// si on doit changer de sens
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1)
				{
					vertices.push_back(new int[2]{ currentX , currentY });
					return;
				}
				currentX++;
				currentY++;
			}
		}
	}
}

void Algo::findChessConnection(const int * Routerfrom, const int * RouterTo, std::vector<int *> & cables) const
{

	// vecteur des cables entre les 2 routeurs aux positions Routerfrom et RouterTo
	const int dx = abs(Routerfrom[0] - RouterTo[0]) + 1;
	const int dy = abs(Routerfrom[1] - RouterTo[1]) + 1;

	const int xmin = std::min(Routerfrom[0], RouterTo[0]);
	const int ymin = std::min(Routerfrom[1], RouterTo[1]);

	// matrice de booleens (0 -> false, 1 -> true)
	Matrix path(dx, dy);

	path(Routerfrom[0] - xmin, Routerfrom[1] - ymin) = 1;
	path(RouterTo[0] - xmin, RouterTo[1] - ymin) = 1;

	const int r[2] = { dx, dy };
	const int amin = argMin(r, 2);


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

	const int rowsPath = path.getRows();
	const int colsPath = path.getCols();
	// on selectionne les cables
	for (int r = 0; r < rowsPath; r++)
	{
		for (int c = 0; c < colsPath; c++)
		{
			// si true dans path, on place un cable a cette coordonnees
			if (path(r, c) == 1)
			{
				cables.push_back(new int[2]{ r + xmin, c + ymin });
			}
		}
	}
}


void Algo::findDiagonalConnection(const int * Routerfrom, const int * RouterTo, std::vector<int *> & cables) const
{
	int distX = abs(Routerfrom[0] - RouterTo[0]);
	int distY = abs(Routerfrom[1] - RouterTo[1]);

	// il y a forcement un cable sur les 2 routeurs
	cables.push_back(new int[2]{ Routerfrom[0],  Routerfrom[1] });
	cables.push_back(new int[2]{ RouterTo[0],  RouterTo[1] });

	int * from1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
	int * to1 = new int[2]{ RouterTo[0], RouterTo[1] };

	// ##########################################
	// on passe par le haut ou par le bas
	// ##########################################
	if (distX + 1 < distY)
	{
		// du routeur le + a gauche vers le routeur le + a droite
		if (Routerfrom[1] > RouterTo[1])
		{
			from1 = new int[2]{ RouterTo[0], RouterTo[1] };
			to1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
		}
		// par le haut
		if (((from1[0] - (distY / 2)) >= 0) && ((to1[0] - (distY / 2)) >= 0))
		{
			int currentX = from1[0] - 1;
			int currentY = from1[1] + 1;

			// on monte
			while (true)
			{

				cables.push_back(new int[2]{ currentX , currentY });

				// si on doit changer de sens

				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1) 
				{
					break;
				}
				currentX--;
				currentY++;
			}

			currentX++;
			currentY++;

			// on descend
			while (true)
			{
				cables.push_back(new int[2]{ currentX , currentY });

				// si on est voisin du routeur de destination
				if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
				{
					break;
				}
				currentX++;
				currentY++;
			}
		}

		// par le bas
		else
		{
			int currentX = from1[0] + 1;
			int currentY = from1[1] + 1;

			// on descend
			while (true)
			{

				cables.push_back(new int[2]{ currentX , currentY });

				// si on doit changer de sens
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1)
				{
					break;
				}
				currentX++;
				currentY++;
			}

			currentX--;
			currentY++;

			// on monte
			while (true)
			{
				cables.push_back(new int[2]{ currentX , currentY });

				// si on est voisin du routeur de destination
				if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
				{
					break;
				}
				currentX--;
				currentY++;
			}
		}
	}

	// ##########################################
	//on passe par la gauche ou par la droite
	// ##########################################
	else if (distX > distY + 1)
	{		
		// du routeur le + en haut vers le routeur le + en bas
		if (Routerfrom[0] > RouterTo[0])
		{
			from1 = new int[2]{ RouterTo[0], RouterTo[1] };
			to1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
		}

		// par la gauche
		if (((from1[1] - (distX / 2)) >= 0) && ((to1[1] - (distX / 2)) >= 0))
		{
			int currentX = from1[0] + 1;
			int currentY = from1[1] - 1;

			// on va a gauche
			while (true)
			{
				cables.push_back(new int[2]{ currentX , currentY });
				
				// si on doit changer de sens
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1) 
				{
					break;
				}
				currentX++;
				currentY--;
			}

			currentX++;
			currentY++;

			// on va a droite
			while (true)
			{
				cables.push_back(new int[2]{ currentX , currentY });

				// si on est voisin du routeur de destination
				if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
				{
					break;
				}
				currentX++;
				currentY++;
			}
		}
		
		// par la droite
		else
		{
			int currentX = from1[0] + 1;
			int currentY = from1[1] + 1;

			// on va a droite
			while (true)
			{
				cables.push_back(new int[2]{ currentX , currentY });

				// si on doit changer de sens
				if (abs(abs(to1[1] - currentY) - abs(to1[0] - currentX)) <= 1) 
				{
					break;
				}
				currentX++;
				currentY++;
			}

			currentX++;
			currentY--;

			// on va a gauche
			while (true)
			{
				cables.push_back(new int[2]{ currentX , currentY });

				// si on est voisin du routeur de destination
				if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
				{
					break;
				}
				currentX++;
				currentY--;
			}
		}
	}

	// ##########################################
	// on tire directement la diagonale
	// ##########################################
	else
	{

		from1 = new int[2]{ Routerfrom[0], Routerfrom[1] };
		to1 = new int[2]{ RouterTo[0], RouterTo[1] };

		// vers le haut
		if (Routerfrom[0] > RouterTo[0])
		{
			// vers la gauche
			if (Routerfrom[1] > RouterTo[1])
			{
				int currentX = from1[0] - 1;
				int currentY = from1[1] - 1;

				while (true)
				{
					cables.push_back(new int[2]{ currentX , currentY });

					// si on est voisin du routeur de destination
					if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
					{
						break;
					}
					currentX--;
					currentY--;
				}
			}
			// vers la droite
			else
			{
				if (Routerfrom[1] > RouterTo[1])
				{
					int currentX = from1[0] - 1;
					int currentY = from1[1] + 1;

					while (true)
					{
						cables.push_back(new int[2]{ currentX , currentY });

						// si on est voisin du routeur de destination
						if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
						{
							break;
						}
						currentX--;
						currentY++;
					}
				}
			}
		}

		// vers le bas
		else
		{
			// vers la gauche
			if (Routerfrom[1] > RouterTo[1])
			{
				int currentX = from1[0] + 1;
				int currentY = from1[1] - 1;

				while (true)
				{
					cables.push_back(new int[2]{ currentX , currentY });

					// si on est voisin du routeur de destination
					if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
					{
						break;
					}
					currentX++;
					currentY--;
				}
			}
			// vers la droite
			else
			{
				if (Routerfrom[1] > RouterTo[1])
				{
					int currentX = from1[0] + 1;
					int currentY = from1[1] + 1;

					while (true)
					{
						cables.push_back(new int[2]{ currentX , currentY });

						// si on est voisin du routeur de destination
						if (chessboardDist(new int[2]{ currentX , currentY }, to1) == 1)
						{
							break;
						}
						currentX++;
						currentY++;
					}
				}
			}
		}
	}

	//delete from1;
	//delete to1;

	// on insert les elements du vecteur cablesTmp dans le vecteur cables
	//cables.insert(std::end(cables), std::begin(cablesTmp), std::end(cablesTmp));


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
void Algo::toCsrMatrix(Matrix & mat, const std::vector<int> & dists, const std::vector<int> & idx, const std::vector<int> & idy, const int dim) const
{
	const int size = dists.size();
	for (int k = 0; k < size; k++)
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
bool Algo::routerOnBackbone(const std::vector<int *> & routeurs) const
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
int Algo::minKey(const std::vector<int> key, const std::vector<bool> mstSet, const int dim) const
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
void Algo::toMinimumSpanningTree(const Matrix & mat, Matrix & mst) const
{


	// dimension de la matrice
	const int dim = mat.getRows();

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
	const int dimMinusOne = dim - 1;
	for (int count = 0; count < dimMinusOne; count++)
	{
		// on choisie le sommet de cle minimum de l'ensemble des sommets par encore inclus dans le mst
		int u = minKey(key, mstSet, dim);

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
			}
		}
	}

	// remplissage de la matrice mst en ne gardant que les arcs de poids minimum
	for (int i = 1; i < dim; i++)
	{

		mst(parent[i], i) = mat(parent[i], i);
	}
}



void Algo::placeMstPaths(const std::vector<int *> & routeurs, const std::vector<int> & idx, const std::vector<int> & idy, const std::vector<int> & dists)
{
	const int dim = routeurs.size();
	// calcul du mst
	Matrix csrMat(dim, dim);
	toCsrMatrix(csrMat, dists, idx, idy, dim);

	// arbre couvrant minimal
	// i.e.  un graphe constitue du sous-ensemble d'aretes qui relient ensemble tous les noeuds connectes, tout en minimisant la somme totale des poids sur les aretes.
	Matrix mstMat(dim, dim);
	toMinimumSpanningTree(csrMat, mstMat);

	const int rows = mstMat.getRows();
	const int cols = mstMat.getCols();
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

	int nbRouters = 0;
	for (int r = 0; r < aMap.getRows(); r++)
	{
		for (int c = 0; c < aMap.getCols(); c++)
		{
			if (aMap(r, c) == Cell::ConnectedRouter)
			{
				nbRouters++;
			}
		}
	}

}

void Algo::placeMstPaths_2(const std::vector<int *> & routeurs, const std::vector<int> & idx, const std::vector<int> & idy, const std::vector<int> & dists)

{

	const int dim = routeurs.size();
	// calcul du mst
	Matrix csrMat(dim, dim);
	toCsrMatrix(csrMat, dists, idx, idy, dim);

	// arbre couvrant minimal
	// i.e.  un graphe constitue du sous-ensemble d'aretes qui relient ensemble tous les noeuds connectes, tout en minimisant la somme totale des poids sur les aretes.
	Matrix mstMat(dim, dim);
	toMinimumSpanningTree(csrMat, mstMat);

	const int rows = mstMat.getRows();
	const int cols = mstMat.getCols();

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

					// si le cable n'est pas sur un routeur
					if (aMap(cable[0], cable[1]) != Cell::ConnectedRouter)
					{
						aMap(cable[0], cable[1]) = Cell::Cable; // on place le cable
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

		if (aMap(rout[0], rout[1]) == Cell::ConnectedRouter || aMap(rout[0], rout[1]) == Cell::Router)
		{
			aMap(rout[0], rout[1]) = Cell::ConnectedRouter; // on passe la valeur de la coordonnee de 2 a 3 pour indiquer que le routeur est connecte au backbone
		}
		else
		{
			aMap(rout[0], rout[1]) = Cell::Cable; // si le sommet est un cable
		}
	}

}


void Algo::placeMstPaths_3(const std::vector<int *> & cables, const std::vector<int *> & vertices)
{

	
	for (int *cable : cables) // pour chaque cable
	{
		// si le cable est sur l'emetteur
		if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
		{
			continue;
		}

		// si le cable n'est pas sur un routeur
		if (aMap(cable[0], cable[1]) != Cell::ConnectedRouter)
		{
			aMap(cable[0], cable[1]) = Cell::Cable; // on place le cable
		}
	}

	for (int * rout : vertices)
	{
		// si le cable est sur l'emetteur
		if (rout[0] == aBackbone[0] && rout[1] == aBackbone[1])
		{
			continue;
		}
	
		aMap(rout[0], rout[1]) = Cell::ConnectedRouter; // on passe la valeur de la coordonnee de 2 a 3 pour indiquer que le routeur est connecte au backbone

	}

}

int Algo::getApproximateCost(const int * newRouteurs, const std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists) const
{
	int distMin = INT_MAX;
	const int new_id = routeurs.size();
	int cpt = 0;

	// on recupere la distance avec le routeur le plus proche
	for (int * rout : routeurs)
	{
		int dist = chessboardDist(rout, newRouteurs);
		if (dist < distMin)
		{
			distMin = dist;
		}
		cpt++;
	}

	return distMin;
}

void Algo::updateApproximateCost(const int * newRouteurs, const std::vector<int *> routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, int & approximateCost) const
{
	int distMin = INT_MAX;
	const int new_id = routeurs.size();
	int cpt = 0;

	// on recupere la distance avec le routeur le plus proche
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
void Algo::kruskal(int * newRouteurs, std::vector<int *> & routeurs, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, bool & succ, int & cost) const
{

	
	const int new_id = routeurs.size();

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

	const int dim = routeurs.size();

	Matrix csrMat(dim, dim);
	toCsrMatrix(csrMat, dists, idx, idy, dim); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
														   // [0 x x 1]
														   // [0 0 x 7]
														   // [0 0 0 2]

														   // arbre couvrant minimal
	Matrix mstMat(dim, dim);
	toMinimumSpanningTree(csrMat, mstMat);

	// poids de l'arbre couvrant minimum. i.e le nombre d'aretes
	const int mstMatWeight = mstMat.sum();


	cost = mstMatWeight * aPrixCable + (new_id) * aPrixRouteur; // routeur - 1 car on interprete le backbone comme un routeur dans l'algorithme

	succ = cost <= aBudgetOriginal; // si on ne depasse par le budget
}
/*
* @param x du routeur
* @param y du routeur
* @param radius rayon du routeur
* @param mat la matrice de la carte
* @return la matrice masque
*/
void Algo::wirelessAccess(const int x, const int y, const Matrix & mat, Matrix & mask) const
{
	const int radius = aRayonRouteurs;
	const int height = mat.getRows();
	const int width = mat.getCols();

	// remplissage de mask
	const int size = mask.getCols() * mask.getRows();
	for (int maskIndex = 0; maskIndex < size; maskIndex++)
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
			if (xt >= height || xt < 0 || yt >= width || yt < 0)
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
			const int toX = std::max(x, xt);
			int fromY = std::min(y, yt);
			const int toY = std::max(y, yt);

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

// routeurs seuls
// nb de cellules couvertes
void Algo::addLastRouters(std::vector<int *> & vertices, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists)
{
	int nbRouters = 0;
	int nbCables = 0;

	const int dim = vertices.size();

	std::vector<int *> routers;

	// calcul du mst
	Matrix csrMat(dim, dim);
	toCsrMatrix(csrMat, dists, idx, idy, dim);


	// arbre couvrant minimal
	// i.e.  un graphe constitue du sous-ensemble d'aretes qui relient ensemble tous les noeuds connectes, tout en minimisant la somme totale des poids sur les aretes.
	Matrix mstMat(dim, dim);
	toMinimumSpanningTree(csrMat, mstMat);

	const int rows = mstMat.getRows();
	const int cols = mstMat.getCols();

	const int aMapRows = aMap.getRows();
	const int aMapCols = aMap.getCols();
	const int aMapSize = aMapRows*aMapCols;

	Matrix cablesMap(aMapRows, aMapCols);

	// algo calcul distance entre les routeurs. Parcours de l'arbre couvrant minimal
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (mstMat(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
			{
				std::vector<int *> cables;
				findChessConnection(vertices[r], vertices[c], cables);

				for (int *cable : cables) // pour chaque cable
				{
					// si le cable est sur l'emetteur
					if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
					{
						continue;
					}

					cablesMap(cable[0], cable[1]) = 1;
				}
			}
		}
	}



	for (int r = 0; r < aMapRows; r++)
	{
		for (int c = 0; c < aMapCols; c++)
		{
			if (aMapSolution(r, c) == Cell::ConnectedRouter)
			{
				nbRouters++;
				routers.push_back(new int[2]{ r, c });
			}
		}
	}

	nbCables = cablesMap.sum();

	// budget restant
	int remainingBudget = aBudgetOriginal - nbRouters * aPrixRouteur - nbCables * aPrixCable;


	// ###########################################################################################################
	// on refait l'algo du flou gaussien pour tenter de placer les derniers routeurs
	// ###########################################################################################################
	
	// nb max de routeurs possibles
	const int maxNumRouters = remainingBudget / aPrixRouteur;

	// matrice avec les cellules cibles a 1 et le reste a -1
	Matrix targetCellsForConvolution(aMapRows, aMapCols);
	// matrice avec a 1 les cellules qui de toute facon n'auront aucun impact sur le flou. On les stocks pour gagner du temps lors des prochaines convolution avec le kernel gaussien
	Matrix deadCellsForConvolution(aMapRows, aMapCols);
	// matrice avec les cellules cibles a 0 et le reste a 1
	Matrix targetCells(aMapRows, aMapCols);
	// nombre de cellules cibles pas encore couvertes
	int nbTargetCells = 0;

	for (int x = 0; x < aMapRows; x++)
	{
		for (int y = 0; y < aMapCols; y++)
		{
			// remplissage de targetCellsForConvolution, de targetCells
			if (aMap(x, y) == Cell::Wireless)
			{
				targetCellsForConvolution(x, y) = 1;
				targetCells(x, y) = 0;
				nbTargetCells++;
				aNbCellsOriginal++;
			}
			else
			{
				targetCellsForConvolution(x, y) = -1;
				targetCells(x, y) = 1;
			}
		}
	}


	// vecteur de float representant un vrai noyau de filtre gaussien. On le manipule comme une matrice
	const int kernelDim = 2 * aRayonRouteurs + 1;
	const int kernelSize = kernelDim * kernelDim;

	std::vector<float> kernel(kernelSize);
	gaussianKernel(kernel, kernelDim);

	int cost = 0; // cout reel calcul d'apres le mst
	bool succ = true;

	for (int * rout : routers)
	{
		// recuperation du masque du perimetre du routeur
		Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

		wirelessAccess(rout[0], rout[1], aMap, mask);

		int indexMask = 0;
		const int xFrom = rout[0] - aRayonRouteurs;
		const int xTo = rout[0] + aRayonRouteurs;
		const int yFrom = rout[1] - aRayonRouteurs;
		const int yTo = rout[1] + aRayonRouteurs;
		// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
		for (int xTargetCell = xFrom; xTargetCell <= xTo; xTargetCell++)
		{
			for (int yTargetCell = yFrom; yTargetCell <= yTo; yTargetCell++)
			{
				// maj de targetCellsForConvolution et de targetCells si nouvelle cellule pas encore couverte
				if (mask(indexMask) == 1 && targetCells(xTargetCell, yTargetCell) == 0)
				{
					targetCellsForConvolution(xTargetCell, yTargetCell) = -1;
					targetCells(xTargetCell, yTargetCell) = 1;
					nbTargetCells--;
				}
				indexMask++;
			}
		}
	}

	for (int currentRouterId = 0; currentRouterId < maxNumRouters; currentRouterId++)
	{
		// si il n'y a pas de position dispo pour le prochain routeur
		if (nbTargetCells == 0)
		{
			for (int * rout : routers)
			{
				aMap(rout[0], rout[1]) = Cell::ConnectedRouter;
			}
			// on tire les cables
			placeMstPaths_2(vertices, idx, idy, dists);
			displayScoreAfterCentroids(targetCells, routers.size());
			return; // et on sort de l'algo
		}

		std::vector<float> convolvedMat(aMapSize);

		// on convolue la matrice de cellules cibles avec le flou gaussien
		convolve(convolvedMat, targetCellsForConvolution, deadCellsForConvolution, kernel, kernelDim);

		// on met la valeur minimum aux cellules qui ne sont pas des cellules cibles pas encore couvertes
		for (int iConv = 0; iConv < aMapSize; iConv++)
		{
			// si la case n'est une cellule cible ou n'apporte pas de nouvelles cellule couvertes /!\ VOIR SI ON A LE DROIT DE METTRE UN ROUTEUR SUR UNE CELLULE VIDE
			if (aMap(iConv) != Cell::Wireless || targetCellsForConvolution(iConv) == -1)
			{
				convolvedMat[iConv] = -kernelSize; // pour ne pas reprendre cette position dans la matrice convoluee
			}
		}

		int * xyNewRouter = new int[2];

		// tant qu'on n'a pas trouve une position pour placer un routeur
		bool findPosition = false;
		while (!findPosition)
		{

			// on recupere les coordonnees avec la plus grande valeur dans le vecteur convolue
			std::vector<int> maxValuesVect; // vecteur des positions dans convolvedMat avec la valeur maximale
			float valMax = -kernelSize; // valeur max de la matrice de convolution. Dans le pire des cas. i.e. avec sigma = 0
			for (int x = 0; x < aMapRows; x++)
			{
				for (int y = 0; y < aMapCols; y++)
				{
					int currentIndex = x * aMapCols + y;
					// si on trouve une valeur + grande, on vide le vecteur et on rajoute les coord
					if (valMax < convolvedMat[currentIndex])
					{
						maxValuesVect.clear();
						maxValuesVect.push_back(currentIndex);
						valMax = convolvedMat[currentIndex];
					}
					// si on trouve une valeur egale, on rajoute les coord au vecteur
					else if (valMax == convolvedMat[currentIndex])
					{
						maxValuesVect.push_back(currentIndex);
					}
				}
			}

			// on recupere les positions qui couvrent un maximum de nouvelles cellules 
			std::vector<int> maxCoveredCellsVect; // vecteur des positions dans convolvedMat avec la valeur maximale en fonction du nombre de cellules nouvellement couvertes
			int nbCells = 0;
			for (int index : maxValuesVect)
			{
				int xIndex = (index / aMapCols);
				int yIndex = (index - xIndex * aMapCols);
				int currentNbCells = nbNewCoveredCells(xIndex, yIndex, targetCells);

				// si on trouve une nb de nouveles cellules couvertes + grand, on vide le vecteur et on rajoute les coord
				if (nbCells < currentNbCells)
				{
					maxCoveredCellsVect.clear();
					maxCoveredCellsVect.push_back(index);
					nbCells = currentNbCells;
				}
				// si on trouve un nb egal, on rajoute les coord au vecteur
				else if (nbCells == currentNbCells)
				{
					maxCoveredCellsVect.push_back(index);
				}
			}

			std::vector<int> minCablesDistVect; // vecteur contenant les meilleures positions pour un nouveau routeur compte tenu du nombre de cellules couvertes
												// et du prix de cablage
			int minCost = INT_MAX;
			// on ne garde que les positions le plus dans un coin

			for (int index : maxCoveredCellsVect)
			{
				int xIndex = (index / aMapCols);
				int yIndex = (index - xIndex * aMapCols);
				int currentCost = getApproximateCost(new int[2]{ xIndex , yIndex }, vertices, idx, idy, dists);

				// si on trouve un cout inferieur, on vide le vecteur et on rajoute les coord
				if (minCost > currentCost)
				{
					minCablesDistVect.clear();
					minCablesDistVect.push_back(index);
					minCost = currentCost;
				}
				// si on trouve un nb egal, on rajoute les coord au vecteur
				else if (minCost == currentCost)
				{
					minCablesDistVect.push_back(index);
				}
			}


			std::vector<int> maxCornersVect; // vecteur contenant les meilleurs positions pour un nouveau routeur compte tenu du nombre de cellules couvertes,
											 // le cout de cablage
											 // et de sa proximite avec des cellules qui n'ont pas besoin d'etre couvertes

			int nbCorners = -1;

			for (int index : minCablesDistVect)
			{
				int xIndex = (index / aMapCols);
				int yIndex = (index - xIndex * aMapCols);
				int currentNbCorners = nbNotTargetCellsAround(xIndex, yIndex, targetCells);

				// si on trouve une nb de coins + grand, on vide le vecteur et on rajoute les coord
				if (currentNbCorners > nbCorners)
				{
					maxCornersVect.clear();
					maxCornersVect.push_back(index);
					nbCorners = currentNbCorners;
				}
				// si on trouve un nb egal, on rajoute les coord au vecteur
				else if (nbCorners == currentNbCorners)
				{
					maxCornersVect.push_back(index);
				}
			}

			// on recupere une position random pour un prochain routeur parmi ces positions optimales
			std::random_device rd;     // seulement utilise une fois pour initialiser le moteur (semence)
			std::mt19937 rng(rd());    // moteur a nombre aleatoire utilise (Mersenne-Twister dans ce cas)
			std::uniform_int_distribution<int> uni(0, aMap.getRows() * aMap.getCols()); // garantie sans biais

			const int random_integer = uni(rng) % maxCornersVect.size();
			int xRouter = (maxCornersVect[random_integer] / aMapCols);
			int yRouter = (maxCornersVect[random_integer] - xRouter * aMapCols);
			xyNewRouter[0] = xRouter;
			xyNewRouter[1] = yRouter;

			// si la case n'est une cellule cible ou n'apporte pas de nouvelles cellule couvertes /!\ VOIR SI ON A LE DROIT DE METTRE UN ROUTEUR SUR UNE CELLULE VIDE
			if (aMap(xRouter, yRouter) != Cell::Wireless || nbCells == 0)
			{
				convolvedMat[maxValuesVect[random_integer]] = -kernelSize; // pour ne pas reprendre cette position dans la matrice convoluee
			}
			else findPosition = true;

		}

		// on garde en memoire l'etat de la case avant d'y mettre un routeur au ca ou celui-ci ne conviendrait pas
		int safeguardingInfo = aMap(xyNewRouter[0], xyNewRouter[1]);

		aMap(xyNewRouter[0], xyNewRouter[1]) = Cell::ConnectedRouter; // Cell::Router = 2. On positionne le routeur sur la carte

																// tentative de placement de ce routeur
																// on sauvegarde l'etat des variables au cas ou on ne pourrait pas placer ce routeur
		std::vector<int> idxTmp = idx;
		std::vector<int> idyTmp = idy;
		std::vector<int> distsTmp = dists;
		int costTmp = cost;

		// ##########################
		// Kruskal
		// ##########################

		const int new_id = vertices.size();

		int cpt = 0;

		// algo calcul distance entre les routeurs
		for (int * rout : vertices)
		{
			int dist = chessboardDist(rout, xyNewRouter);
			if (dist > 0)
			{
				idx.push_back(cpt);
				idy.push_back(new_id);
				dists.push_back(dist);
			}
			cpt++;
		}

		routers.push_back(xyNewRouter);
		vertices.push_back(xyNewRouter);

		const int dim = vertices.size();

		Matrix csrMat(dim, dim);
		toCsrMatrix(csrMat, dists, idx, idy, dim); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
												   // [0 x x 1]
												   // [0 0 x 7]
												   // [0 0 0 2]

												   // arbre couvrant minimal
		Matrix mstMat(dim, dim);
		toMinimumSpanningTree(csrMat, mstMat);

		Matrix cablesM(aMapRows, aMapCols);

		for (int r = 0; r < dim; r++)
		{
			for (int c = 0; c < dim; c++)
			{
				if (mstMat(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
				{
					std::vector<int *> cables;
					findChessConnection(vertices[r], vertices[c], cables);

					for (int *cable : cables) // pour chaque cable
					{
						// si le cable est sur l'emetteur
						if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
						{
							continue;
						}

						// si le cable n'est pas sur un routeur
						if (aMap(cable[0], cable[1]) != Cell::ConnectedRouter)
						{
							cablesM(cable[0], cable[1]) = 1; // on place le cable
						}
					}
				}
			}
		}

		cost = cablesM.sum() * aPrixCable + routers.size() * aPrixRouteur; // routeur - 1 car on interprete le backbone comme un routeur dans l'algorithme
		succ = cost <= aBudgetOriginal; // si on ne depasse par le budget


		// si encore du budget
		if (succ)
		{
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(xyNewRouter[0], xyNewRouter[1], aMap, mask);

			int indexMask = 0;
			const int xFrom = xyNewRouter[0] - aRayonRouteurs;
			const int xTo = xyNewRouter[0] + aRayonRouteurs;
			const int yFrom = xyNewRouter[1] - aRayonRouteurs;
			const int yTo = xyNewRouter[1] + aRayonRouteurs;

			// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
			for (int xTargetCell = xFrom; xTargetCell <= xTo; xTargetCell++)
			{
				for (int yTargetCell = yFrom; yTargetCell <= yTo; yTargetCell++)
				{
					// maj de targetCellsForConvolution et de targetCells si nouvelle cellule pas encore couverte
					if (mask(indexMask) == 1 && targetCells(xTargetCell, yTargetCell) == 0)
					{
						targetCellsForConvolution(xTargetCell, yTargetCell) = -1;
						targetCells(xTargetCell, yTargetCell) = 1;
						nbTargetCells--;
					}
					indexMask++;
				}
			}
		}
		else // plus de budget, on enleve le routeur en trop
		{

			// on retabli l'etat de la case dans la carte
			aMap(xyNewRouter[0], xyNewRouter[1]) = safeguardingInfo;

			// on supprime le routeur de trop du vecteur
			routers.erase(routers.begin() + routers.size() - 1);
			vertices.erase(vertices.begin() + vertices.size() - 1);

			// on reduit la taille du vecteur au nombre d'elements dans le vecteur
			routers.shrink_to_fit();
			vertices.shrink_to_fit();

			for (int * rout : routers)
			{
				aMap(rout[0], rout[1]) = Cell::ConnectedRouter;
			}

			// on tire les cables
			placeMstPaths_2(vertices, idxTmp, idyTmp, distsTmp);

			displayScoreAfterCentroids(targetCells, routers.size());

			return;

		}

		//targetCellsCoords.clear(); // on vide le vecteur, ce qui aura effet de delete les new int[2] (eviter les fuites memoires)
		//delete xyNewRouter;
	}

	for (int * rout : routers)
	{
		aMap(rout[0], rout[1]) = Cell::ConnectedRouter;
	}

	placeMstPaths_2(vertices, idx, idy, dists);

	displayScoreAfterCentroids(targetCells, routers.size());
}

int Algo::centerIsBetter(int * center, std::vector<int *> & vertices, std::vector<int> & idx, std::vector<int> & idy, std::vector<int> & dists, const int nbCablesBefore, Matrix & intermediateCsrMat)
{
	std::vector<int> idxTmp = idx;
	std::vector<int> idyTmp = idy;
	std::vector<int> distsTmp = dists;

	const int new_id = vertices.size();

	int cpt = 0;

	// algo calcul distance entre les routeurs
	for (int * ver : vertices)
	{
		int dist = chessboardDist(ver, center);
		if (dist > 0)
		{
			idxTmp.push_back(cpt); // ex: idx = [0, 1, 2]
			idyTmp.push_back(new_id); // ex: idy = [3, 3, 3]
			distsTmp.push_back(dist); // ex: dists = [1, 7, 4]
		}
		cpt++;
	}

	std::vector<int *> verticesTmp = vertices;
	verticesTmp.push_back(center);

	const int dim = verticesTmp.size();

	Matrix csrMat(dim, dim);
	toCsrMatrix(csrMat, distsTmp, idxTmp, idyTmp, dim); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
											   // [0 0 0 1]
											   // [0 0 0 7]
											   // [0 0 0 4]
											   // [1 7 4 0]

	// arbre couvrant minimal
	Matrix mstMat(dim, dim);
	toMinimumSpanningTree(csrMat, mstMat);

	// poids de l'arbre couvrant minimum. i.e le nombre d'aretes
	const int mstMatWeight = mstMat.sum();

	const int diff = nbCablesBefore - mstMatWeight;

	// si ce cablage est demande moins de cables, on valide les variables temporaires
	if (diff > 0)
	{
		idx = idxTmp;
		idy = idyTmp;
		dists = distsTmp;
		vertices = verticesTmp;
		intermediateCsrMat = csrMat;
	}
	return diff;
}


float * Algo::get3rdPointEquilateral(const int * A, const int * B, const int * C) const
{
	const float PI = 3.14159265;

	float sin60 = sin(60 * PI / 180.0);
	float cos60 = cos(60 * PI / 180.0);

	// une rotation horaire
	float XclockwiseRotation = cos60 * (A[0] - B[0]) + sin60 * (A[1] - B[1]) + B[0];
	float YclockwiseRotation = cos60 * (A[1] - B[1]) - sin60 * (A[0] - B[0]) + B[1];

	// rotation anti-horaire
	float XantiClockwiseRotation = cos60 * (A[0] - B[0]) - sin60 * (A[1] - B[1]) + B[0];
	float YantiClockwiseRotation = cos60 * (A[1] - B[1]) + sin60 * (A[0] - B[0]) + B[1];

	// calcul de la distance des 2 points par rapport au point C. 
	int distClockwiseRotation = sqrt(pow((XclockwiseRotation - C[0]), 2) + (pow((YclockwiseRotation - C[1]), 2)));
	int distAntiClockwiseRotation = sqrt(pow((XantiClockwiseRotation - C[0]), 2) + (pow((YantiClockwiseRotation - C[1]), 2)));

	// on renvoie les coordonnees du point le plus eloigne de C
	return distClockwiseRotation > distAntiClockwiseRotation ? new float[2]{ XclockwiseRotation , YclockwiseRotation } : new float[2]{ XantiClockwiseRotation , YantiClockwiseRotation };
}

/*
* renvoie la barycentre d'un triangle
*
* @param pt1 le 1er point du triangle
* @param pt2 le 2eme point du triangle
* @param pt3 le 3eme point du triangle
* @return le point representant le centre de gravite
*/
int * Algo::getCentroid(const int * A, const int * B, const int * C) const
{

	int centerX = 0;
	int centerY = 0;

	const float PI = 3.14159265;
	// on multiplie les coordonnees des points par 1000 car l'algorithme marche mal avec des petits nombres
	int distAB = sqrt(pow((A[0] * 1000 - B[0] * 1000), 2) + (pow((A[1] * 1000 - B[1] * 1000), 2)));
	int distBC = sqrt(pow((B[0] * 1000 - C[0] * 1000), 2) + (pow((B[1] * 1000 - C[1] * 1000), 2)));
	int distCA = sqrt(pow((A[0] * 1000 - C[0] * 1000), 2) + (pow((A[1] * 1000 - C[1] * 1000), 2)));

	// calcule des angles du triangle ABC
	float angle_ABC = acos((pow(distAB, 2) + pow(distBC, 2) - pow(distCA, 2)) / (2 * distAB*distBC)) * (180 / PI);
	float angle_BCA = acos((pow(distBC, 2) + pow(distCA, 2) - pow(distAB, 2)) / (2 * distBC*distCA)) * (180 / PI);
	float angle_CAB = acos((pow(distCA, 2) + pow(distAB, 2) - pow(distBC, 2)) / (2 * distCA*distAB)) * (180 / PI);



	// si tous les angles sont < 120° 
	if (angle_ABC < 120 && angle_BCA < 120 && angle_CAB < 120)
	{

		// on calcule le point de Fermat
		// soit ABC un triangle dont les angles sont inferieurs à 120°. Il existe un et un seul point I, dont la somme IA + IB + IC des distances aux trois sommets est minimale.
		// si ABC est borde extérieurement par trois triangles equilateraux ABD, BCE et CAF, les segments [AE], [BF] et [CD] sont concourants en I (le premier centre isogonique du triangle)

		// milieu du segment [AB]
		int * middleAB = new int[2]{ (A[0] + B[0]) / 2, (A[1] + B[1]) / 2 };
		// milieu du segment [BC]
		int * middleBC = new int[2]{ (B[0] + C[0]) / 2, (B[1] + C[1]) / 2 };
		// position du 3eme point du triangle equilateral ABD
		float * D = new float[2];
		// position du 3eme point du triangle equilateral BCE
		float * E = new float[2];

		D = get3rdPointEquilateral(A, B, C);
		E = get3rdPointEquilateral(B, C, A);

		// le point de Fermat est le point a l'intersection entre les droites [AD] et [BE]



		std::vector<float> vectC(3);
		vectC[0] = C[0];
		vectC[1] = C[1];
		vectC[2] = 1;
		std::vector<float> vectD(3);
		vectD[0] = D[0];
		vectD[1] = D[1];
		vectD[2] = 1;
		std::vector<float> vectA(3);
		vectA[0] = A[0];
		vectA[1] = A[1];
		vectA[2] = 1;
		std::vector<float> vectE(3);
		vectE[0] = E[0];
		vectE[1] = E[1];
		vectE[2] = 1;

		std::vector<float> vectCD(3);
		vectCD[0] = vectC[1] * vectD[2] - vectC[2] * vectD[1];
		vectCD[1] = vectC[0] * vectD[2] - vectC[2] * vectD[0];
		vectCD[2] = vectC[0] * vectD[1] - vectC[1] * vectD[0];
		std::vector<float> vectAE(3);
		vectAE[0] = vectA[1] * vectE[2] - vectA[2] * vectE[1];
		vectAE[1] = vectA[0] * vectE[2] - vectA[2] * vectE[0];
		vectAE[2] = vectA[0] * vectE[1] - vectA[1] * vectE[0];

		std::vector<float> vectCDAE(3);
		vectCDAE[0] = vectCD[1] * vectAE[2] - vectCD[2] * vectAE[1];
		vectCDAE[1] = vectCD[0] * vectAE[2] - vectCD[2] * vectAE[0];
		vectCDAE[2] = vectCD[0] * vectAE[1] - vectCD[1] * vectAE[0];

		// le point de Fermat
		centerX = std::round(vectCDAE[0] / vectCDAE[2]);
		centerY = std::round(vectCDAE[1] / vectCDAE[2]);
	}
	// sinon, on prend le point barycentre equilibrant la masse du triangle
	else
	{
		centerX = std::round((A[0] + B[0] + C[0]) / 3);
		centerY = std::round((A[1] + B[1] + C[1]) / 3);
	}


	return new int[2]{ centerX, centerY };
}


void Algo::centroid(std::vector<int *> & routeurs, IO & io, std::string pathBeginning)
{

	// vecteur des sommets du graphe. Au debut, il n'y a que le backbone + les routeurs
	std::vector<int *> vertices;

	// #######################################################################################
	// remplissage de idx, idy et dists pour les prochains Kruskal 
	// #######################################################################################

	std::vector<int> idx;
	std::vector<int> idy;
	std::vector<int> dists;

	const int routeursSize = routeurs.size();

	int new_id = 0;

	for (int * rout1 : routeurs)
	{
		int cpt = 0;
		// calcul distance entre les routeurs
		for (int * rout2 : routeurs)
		{
			int dist = chessboardDist(rout1, rout2);
			if (dist > 0)
			{
				idx.push_back(cpt); // ex: idx = [0, 1, 2]
				idy.push_back(new_id); // ex: idy = [3, 3, 3]
				dists.push_back(dist); // ex: dists = [1, 7, 4]
			}
			cpt++;
		}

		vertices.push_back(rout1);
		new_id++;
	}


	// #######################################################################################
	// matrice d'adjacence
	// #######################################################################################

	Matrix csrMat(routeursSize, routeursSize);
	toCsrMatrix(csrMat, dists, idx, idy, routeursSize);

	// #######################################################################################
	// matrice d'adjacence de l'arbre couvrant minimal
	// #######################################################################################

	// arbre couvrant minimal
	Matrix mstMat(routeursSize, routeursSize);
	toMinimumSpanningTree(csrMat, mstMat);




	/*

	// poids de l'arbre couvrant minimum. i.e le nombre d'aretes
	const int originalWeight = mstMat.sum();

	// #######################################################################################
	// calcul des barycentres pour chercher a optimiser le cablage
	// #######################################################################################

	int newDistSum = originalWeight;
	const int dim = csrMat.getRows();

	std::vector<int> availableVerticesIndexs(dim);
	int cpt = 0;
	for (int index = 0; index < dim; index++ )
	{
		availableVerticesIndexs[index] = cpt;
		cpt++;
	}
	
	while (availableVerticesIndexs.size())
	{
		// on recupere une position random pour un pour construire le + petit carré
		std::random_device rd;     // seulement utilise une fois pour initialiser le moteur (semence)
		std::mt19937 rng(rd());    // moteur a nombre aleatoire utilise (Mersenne-Twister dans ce cas)
		std::uniform_int_distribution<int> uni(0, dim); // garantie sans biais

		auto random_integer = uni(rng) % availableVerticesIndexs.size();
		int indexVertice = random_integer;

		int distToA = INT_MAX;
		int distToB = INT_MAX;
		int indexVerticeA;
		int indexVerticeB;

		// parcours de la matrice d'adjacence du sommet indexVertice pour trouver les 2 sommets les + proches 
		// afin de calculer le barycentre du traingle forme par le sommet indexVertice et des 2 autres sommets les + proches de lui
		for (int verticeCol = 0; verticeCol < csrMat.getCols(); verticeCol++)
		{
			if (indexVertice != verticeCol) // on ne regarde pas les diagonales
			{
				int currentDist = csrMat(indexVertice, verticeCol);

				// si la distance avec ce point est < a la distance avec le point indexA
				if (currentDist < distToA)
				{
					distToA = currentDist;
					indexVerticeA = verticeCol;
				}
				// sinon si la distance avec ce point est < a la distance avec le point indexB
				else if (currentDist < distToB)
				{
					distToB = currentDist;
					indexVerticeB = verticeCol;
				}
			}
		}


		// on recupere la position du barycentre du triangle forme par ces 3 points
		//int * center = getCentroid(new int[2]{ 2,5 }, new int[2]{ 1,1 }, new int[2]{ 3,3 });
		int * center = getCentroid(vertices[random_integer], vertices[indexVerticeA], vertices[indexVerticeB]);

		// on regarde si ajouter ce sommet permet d'utiliser moins de cable
		int diffNbCables = centerIsBetter(center, vertices, idx, idy, dists, newDistSum, csrMat); // modifie vertices, idx, idyTmp, dists et csrMat si, apres Kruskal, diff > 0

		if (diffNbCables > 0)
		{
			// on maj le nombre de cables courant
			newDistSum -= diffNbCables;
		}
		
		// on supprime le sommet du vecteur
		availableVerticesIndexs.erase(availableVerticesIndexs.begin() + indexVertice);

		// on reduit la taille du vecteur au nombre d'elements dans le vecteur
		availableVerticesIndexs.shrink_to_fit();

	}

	*/

	const int dim = csrMat.getRows();
	// si la nouvelle distance inter-routeurs est inferieure la distance inter-routeurs precedente
	//if (originalWeight > newDistSum)
	if (true)
	{

		// #######################################################################################
		// affichage du nombre de cables
		// #######################################################################################
		const int aMapRows = aMap.getRows();
		const int aMapCols = aMap.getCols();
		Matrix cablesMap(aMapRows, aMapCols);
		Matrix cablesMap2(aMapRows, aMapCols);
		int nbCables = 0;
		int nbCables2 = 0;

		// on ajoute les intersect dans l'arbre couvrant minimal
		for (int r = 0; r < dim; r++)
		{
			for (int c = 0; c < dim; c++)
			{
				if (mstMat(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
				{
					addIntersectInVertices(routeurs[r], routeurs[c], vertices);
				}
			}
		}


		// on reconstruit l'arbre couvrant minimal
		idx.clear();
		idy.clear();
		dists.clear();

		const int routeursSize2 = vertices.size();

		int new_id = 0;

		for (int * rout1 : vertices)
		{
			int cpt = 0;
			// calcul distance entre les routeurs
			for (int * rout2 : vertices)
			{
				int dist = chessboardDist(rout1, rout2);
				if (dist > 0)
				{
					idx.push_back(cpt);
					idy.push_back(new_id);
					dists.push_back(dist);
				}
				cpt++;
			}
			new_id++;
		}


		// #######################################################################################
		// matrice d'adjacence
		// #######################################################################################

		Matrix csrMat2(routeursSize2, routeursSize2);
		toCsrMatrix(csrMat2, dists, idx, idy, routeursSize2);

		// #######################################################################################
		// matrice d'adjacence de l'arbre couvrant minimal
		// #######################################################################################

		// arbre couvrant minimal
		Matrix mstMat2(routeursSize2, routeursSize2);
		toMinimumSpanningTree(csrMat2, mstMat2);

		std::vector<int *> allCables;
		std::vector<int *> allCables2 = allCables;

		const int dim2 = csrMat2.getRows();

		for (int r = 0; r < dim2; r++)
		{
			for (int c = 0; c < dim2; c++)
			{
				if (mstMat2(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
				{
					std::vector<int *> cables;
					findChessConnection(vertices[r], vertices[c], cables);
					allCables.insert(std::end(allCables), std::begin(cables), std::end(cables));

					for (int *cable : cables) // pour chaque cable
					{
						// si le cable est sur l'emetteur
						if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
						{
							continue;
						}

						cablesMap(cable[0], cable[1]) = Cell::Cable;
						cablesMap2(cable[0], cable[1]) = 1;
					}
				}
			}
		}




		for (int r = 0; r < aMapRows; r++)
		{
			for (int c = 0; c < aMapCols; c++)
			{
				if (aMapSolution(r, c) == Cell::ConnectedRouter)
				{
					cablesMap(r, c) = Cell::Cable;
				}
			}
		}

		for (int r = 0; r < aMapRows; r++)
		{
			for (int c = 0; c < aMapCols; c++)
			{
				if (cablesMap(r, c) == Cell::Cable)
				{
					nbCables++;
				}
			}
		}


/*
		Matrix aMapTmp = aMap;
		placeMstPaths_2(routeurs, idx, idy, dists);

		// on sauvegarde l'avancement en generant l'output
		io.generateOutput(aMap, pathBeginning);
		
		aMap = aMapTmp;
		*/
		// on re essaie de placer des points barycentre
		//centroid(vertices, io,pathBeginning);

		std::vector<int *> onlyRouters;
		int nbRouteurs = 0;

		for (int r = 0; r < aMapRows; r++)
		{
			for (int c = 0; c < aMapCols; c++)
			{
				if (aMapSolution(r, c) == Cell::ConnectedRouter)
				{
					nbRouteurs++;
					onlyRouters.push_back(new int[2]{ r, c });
				}
			}
		}


		nbCables2 = cablesMap2.sum();

		std::vector<int *> updatedVertices;
		updatedVertices = vertices;
		int indexRout = nbRouteurs+1;

		int newNbCables = 0;

		// on enleve les intersections si cela fait gagner du cout de cablage
		while(indexRout < updatedVertices.size())
		{

			std::vector<int *> updatedVerticesTmp;
			updatedVerticesTmp = updatedVertices;

			updatedVerticesTmp.erase(updatedVerticesTmp.begin() + indexRout);
			// on reconstruit l'arbre couvrant minimal
			idx.clear();
			idy.clear();
			dists.clear();

			const int routeursSize3 = updatedVerticesTmp.size();

			int new_id = 0;

			for (int * rout1 : updatedVerticesTmp)
			{
				int cpt = 0;
				// calcul distance entre les routeurs
				for (int * rout2 : updatedVerticesTmp)
				{
					int dist = chessboardDist(rout1, rout2);
					if (dist > 0)
					{
						idx.push_back(cpt);
						idy.push_back(new_id);
						dists.push_back(dist);
					}
					cpt++;
				}
				new_id++;
			}


			// #######################################################################################
			// matrice d'adjacence
			// #######################################################################################

			Matrix csrMat3(routeursSize3, routeursSize3);
			toCsrMatrix(csrMat3, dists, idx, idy, routeursSize3);

			// #######################################################################################
			// matrice d'adjacence de l'arbre couvrant minimal
			// #######################################################################################

			// arbre couvrant minimal
			Matrix mstMat3(routeursSize3, routeursSize3);
			toMinimumSpanningTree(csrMat3, mstMat3);

			Matrix cablesMap3(aMapRows, aMapCols);

			std::vector<int *> allCables3;

			const int dim3 = csrMat3.getRows();

			for (int r = 0; r < dim3; r++)
			{
				for (int c = 0; c < dim3; c++)
				{
					if (mstMat3(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
					{
						std::vector<int *> cables;
						findChessConnection(updatedVerticesTmp[r], updatedVerticesTmp[c], cables);
						allCables3.insert(std::end(allCables3), std::begin(cables), std::end(cables));

						for (int *cable : cables) // pour chaque cable
						{
							// si le cable est sur l'emetteur
							if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
							{
								continue;
							}

							cablesMap3(cable[0], cable[1]) = 1;
						}
					}
				}
			}

			newNbCables = cablesMap3.sum();

			if (newNbCables < nbCables2)
			{
				nbCables2 = newNbCables;
				updatedVertices = updatedVerticesTmp;
				allCables2 = allCables3;
			}
			else
			{
				indexRout++;
			}
		}


		placeMstPaths_3(allCables2, onlyRouters);
		
	}
	else
	{


		// #######################################################################################
		// affichage du nombre de cables
		// #######################################################################################
		// ##########################
		// Kruskal
		// ##########################

		const int size = vertices.size();
		const int aMapRows = aMap.getRows();
		const int aMapCols = aMap.getCols();

		Matrix csrMat2(size, size);
		toCsrMatrix(csrMat2, dists, idx, idy, size); // d'apres l'ex avec les donnees ci-dessus, donnerait la matrice: 
												   // [0 x x 1]
												   // [0 0 x 7]
												   // [0 0 0 2]

		// arbre couvrant minimal
		Matrix mstMat2(size, size);
		toMinimumSpanningTree(csrMat2, mstMat2);

		Matrix cablesM(aMapRows, aMapCols);

		for (int r = 0; r < size; r++)
		{
			for (int c = 0; c < size; c++)
			{
				if (mstMat2(r, c) > 0) // si [r, c] un somment de l'arbre couvrant minimal
				{
					std::vector<int *> cables;
					findChessConnection(vertices[r], vertices[c], cables);

					for (int *cable : cables) // pour chaque cable
					{
						// si le cable est sur l'emetteur
						if (cable[0] == aBackbone[0] && cable[1] == aBackbone[1])
						{
							continue;
						}	
						cablesM(cable[0], cable[1]) = 1; // on place le cable
					}
				}
			}
		}

		for (int r = 0; r < aMapRows; r++)
		{
			for (int c = 0; c < aMapCols; c++)
			{
				if (aMapSolution(r, c) == Cell::ConnectedRouter)
				{
					cablesM(r, c) = 1;
				}
			}
		}

		// si c'est une map ou il est impossible de couvrir 100% des cellules cibles
		// l'optimisation du cablage a pu liberer du budget
		// on essaie donc d'ajouter d'autres routeurs pour couvrir davantage de cellules
		if (aMapName == "rue_de_londres" || aMapName == "opera")
		{
			addLastRouters(vertices, idx, idy, dists);
		}
		else
		{
			aMap = aMapSolution;
			placeMstPaths_2(routeurs, idx, idy, dists);
		}
	}
}

/*
* defini aleatoirement la position des routeurs
*/
void Algo::random()
{
	// nb max de routeurs possibles
	int maxNumRouters = aBudget / aPrixRouteur;

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
			if (approximateCost >= aBudgetOriginal)
			{
				useKruskal = true;
				kruskal(xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists

			}
			else
			{
				routeurs.push_back(xyNewRouter);
			}
		}
		else {
			kruskal(xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists
		}


		// si encore du budget
		if (succ)
		{
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(xyNewRouter[0], xyNewRouter[1], aMap, mask);

			int indexMask = 0;
			// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
			int condition3 = xyNewRouter[0] + aRayonRouteurs;
			for (int xTargetCell = xyNewRouter[0] - aRayonRouteurs; xTargetCell <= condition3; xTargetCell++)
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
				std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << (useKruskal ? cost : approximateCost) << " / " << aBudgetOriginal << " = " << ((useKruskal ? cost : approximateCost) * 100) / aBudgetOriginal << "%" << (useKruskal ? "\t" : "") << "\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
				std::cout << '\r';

			}
		}
		else // plus de budget, on enleve le routeur en trop
		{

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
		// vecteur des coordonnees des cellules cibles n'ayant pas encore de routeur place dessus
		std::vector<int *> targetCellsCoords;
		int bestMove = 0, otherMove = 0;
		int * theChosenOne = 0;
		int condition2 = targetCells.getRows() * targetCells.getCols();
		for (int cellxy = 0; cellxy < condition2; cellxy++)
		{
			if (targetCells(cellxy) == 0) {
				targetCellsCoords.push_back(targetCells.xy(cellxy));
			}
		}
		int sizeTestCells = targetCellsCoords.size();
		for (int cellxy = 0; cellxy < sizeTestCells; cellxy++)
		{
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);
			otherMove = gainPoints(targetCellsCoords[cellxy][0], targetCellsCoords[cellxy][1], aRayonRouteurs, aMap, mask);
			int indexMask = 0;
			int condition3 = targetCellsCoords[cellxy][0] + aRayonRouteurs;
			int recount = 0;
			for (int xTargetCell = targetCellsCoords[cellxy][0] - aRayonRouteurs; xTargetCell <= condition3; xTargetCell++)
			{
				int condition4 = targetCellsCoords[cellxy][1] + aRayonRouteurs;
				for (int yTargetCell = targetCellsCoords[cellxy][1] - aRayonRouteurs; yTargetCell <= condition4; yTargetCell++)
				{
					if (aMap(indexMask) == 1)
					{
						recount++;
					}
					indexMask++;
				}
			}

			if ((otherMove - recount) > bestMove) {
				theChosenOne = targetCellsCoords[cellxy];
				bestMove = otherMove - recount;
			}
		}

		// si il n'y a pas de position dispo pour le prochain routeur
		if (targetCellsCoords.size() == 0)
		{

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
				kruskal(theChosenOne, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists

			}
			else
			{
				routeurs.push_back(theChosenOne);
			}
		}
		else {
			kruskal(theChosenOne, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists
		}

		// si encore du budget
		if (succ)
		{
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(theChosenOne[0], theChosenOne[1], aMap, mask);

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
}

/*
* defini aleatoirement la position des routeurs
*/
void Algo::gaussianBlur()
{
	
	// nb max de routeurs possibles
	const int maxNumRouters = aBudget / aPrixRouteur;
	const int aMapRows = aMap.getRows();
	const int aMapCols = aMap.getCols();
	const int aMapSize = aMapRows*aMapCols;
	std::cout << "  --------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "  Routeurs\t\t|\t\t% budget utilise\t\t|\t\t% cellules couvertes" << std::endl;
	std::cout << "  --------------------------------------------------------------------------------------------------------------------" << std::endl;


	// matrice avec les cellules cibles a 1 et le reste a -1
	Matrix targetCellsForConvolution(aMapRows, aMapCols);
	// matrice avec a 1 les cellules qui de toute facon n'auront aucun impact sur le flou. On les stocks pour gagner du temps lors des prochaines convolution avec le kernel gaussien
	Matrix deadCellsForConvolution(aMapRows, aMapCols);
	// matrice avec les cellules cibles a 0 et le reste a 1
	Matrix targetCells(aMapRows, aMapCols);
	// nombre de cellules cibles pas encore couvertes
	int nbTargetCells = 0;

	for (int x = 0; x < aMapRows; x++)
	{
		for (int y = 0; y < aMapCols; y++)
		{
			// remplissage de targetCellsForConvolution, de targetCells
			if (aMap(x, y) == Cell::Wireless)
			{
				targetCellsForConvolution(x, y) = 1;
				targetCells(x, y) = 0;
				nbTargetCells++;
				aNbCellsOriginal++;
			}
			else
			{
				targetCellsForConvolution(x, y) = -1;
				targetCells(x, y) = 1;
			}
		}
	}


	// vecteur de float representant un vrai noyau de filtre gaussien. On le manipule comme une matrice
	const int kernelDim = 2 * aRayonRouteurs + 1;
	const int kernelSize = kernelDim * kernelDim;

	std::vector<float> kernel(kernelSize);
	gaussianKernel(kernel, kernelDim);

	//displayVector(kernel, 2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

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
		// si il n'y a pas de position dispo pour le prochain routeur
		if (nbTargetCells == 0)
		{

			// on tire les cables
			placeMstPaths(routeurs, idx, idy, dists);
			std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << approximateCost << " / " << aBudgetOriginal << " = " << (approximateCost * 100) / aBudgetOriginal << "%" << "\t\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
			std::cout << '\r';
			displayScore(targetCells, routeurs.size() - 1);
			return; // et on sort de l'algo
		}

		std::vector<float> convolvedMat(aMapSize);

		// on convolue la matrice de cellules cibles avec le flou gaussien
		convolve(convolvedMat, targetCellsForConvolution, deadCellsForConvolution, kernel, kernelDim);

		// on met la valeur minimum aux cellules qui ne sont pas des cellules cibles pas encore couvertes
		for (int iConv = 0; iConv < aMapSize; iConv++)
		{
			// si la case n'est une cellule cible ou n'apporte pas de nouvelles cellule couvertes /!\ VOIR SI ON A LE DROIT DE METTRE UN ROUTEUR SUR UNE CELLULE VIDE
			if (aMap(iConv) != Cell::Wireless || targetCellsForConvolution(iConv) == -1)
			{
				convolvedMat[iConv] = -kernelSize; // pour ne pas reprendre cette position dans la matrice convoluee
			}
		}

		int * xyNewRouter = new int[2];

		// tant qu'on n'a pas trouve une position pour placer un routeur
		bool findPosition = false;
		while (!findPosition)
		{
			// on recupere les coordonnees avec la plus grande valeur dans le vecteur convolue
			std::vector<int> maxValuesVect; // vecteur des positions dans convolvedMat avec la valeur maximale
			float valMax = -kernelSize; // valeur max de la matrice de convolution. Dans le pire des cas. i.e. avec sigma = 0
			for (int x = 0; x < aMapRows; x++)
			{
				for (int y = 0; y < aMapCols; y++)
				{
					int currentIndex = x * aMapCols + y;
					// si on trouve une valeur + grande, on vide le vecteur et on rajoute les coord
					if (valMax < convolvedMat[currentIndex])
					{
						maxValuesVect.clear();
						maxValuesVect.push_back(currentIndex);
						valMax = convolvedMat[currentIndex];
					}
					// si on trouve une valeur egale, on rajoute les coord au vecteur
					else if (valMax == convolvedMat[currentIndex])
					{
						maxValuesVect.push_back(currentIndex);
					}
				}
			}

			// on recupere les positions qui couvrent un maximum de nouvelles cellules 
			std::vector<int> maxCoveredCellsVect; // vecteur des positions dans convolvedMat avec la valeur maximale en fonction du nombre de cellules nouvellement couvertes
			int nbCells = 0;
			for (int index : maxValuesVect)
			{
				int xIndex = (index / aMapCols);
				int yIndex = (index - xIndex * aMapCols);
				int currentNbCells = nbNewCoveredCells(xIndex, yIndex, targetCells);

				// si on trouve une nb de nouveles cellules couvertes + grand, on vide le vecteur et on rajoute les coord
				if (nbCells < currentNbCells)
				{
					maxCoveredCellsVect.clear();
					maxCoveredCellsVect.push_back(index);
					nbCells = currentNbCells;
				}
				// si on trouve un nb egal, on rajoute les coord au vecteur
				else if (nbCells == currentNbCells)
				{
					maxCoveredCellsVect.push_back(index);
				}
			}

			std::vector<int> minCablesDistVect; // vecteur contenant les meilleures positions pour un nouveau routeur compte tenu du nombre de cellules couvertes
											// et du prix de cablage
			int minCost = INT_MAX;
			// on ne garde que les positions le plus dans un coin

			for (int index : maxCoveredCellsVect)
			{
				int xIndex = (index / aMapCols);
				int yIndex = (index - xIndex * aMapCols);
				int currentCost = getApproximateCost(new int[2]{ xIndex , yIndex }, routeurs, idx, idy, dists);
				
				// si on trouve un cout inferieur, on vide le vecteur et on rajoute les coord
				if (minCost > currentCost)
				{
					minCablesDistVect.clear();
					minCablesDistVect.push_back(index);
					minCost = currentCost;
				}
				// si on trouve un nb egal, on rajoute les coord au vecteur
				else if (minCost == currentCost)
				{
					minCablesDistVect.push_back(index);
				}
			}


			std::vector<int> maxCornersVect; // vecteur contenant les meilleurs positions pour un nouveau routeur compte tenu du nombre de cellules couvertes,
												// le cout de cablage
												// et de sa proximite avec des cellules qui n'ont pas besoin d'etre couvertes

			int nbCorners = -1;

			for (int index : minCablesDistVect)
			{
				int xIndex = (index / aMapCols);
				int yIndex = (index - xIndex * aMapCols);
				int currentNbCorners = nbNotTargetCellsAround(xIndex, yIndex, targetCells);

				// si on trouve une nb de coins + grand, on vide le vecteur et on rajoute les coord
				if (currentNbCorners > nbCorners)
				{
					maxCornersVect.clear();
					maxCornersVect.push_back(index);
					nbCorners = currentNbCorners;
				}
				// si on trouve un nb egal, on rajoute les coord au vecteur
				else if (nbCorners == currentNbCorners)
				{
					maxCornersVect.push_back(index);
				}
			}

			// on recupere une position random pour un prochain routeur parmi ces positions optimales
			std::random_device rd;     // seulement utilise une fois pour initialiser le moteur (semence)
			std::mt19937 rng(rd());    // moteur a nombre aleatoire utilise (Mersenne-Twister dans ce cas)
			std::uniform_int_distribution<int> uni(0, aMap.getRows() * aMap.getCols()); // garantie sans biais

			const int random_integer = uni(rng) % maxCornersVect.size();
			int xRouter = (maxCornersVect[random_integer] / aMapCols);
			int yRouter = (maxCornersVect[random_integer] - xRouter * aMapCols);
			xyNewRouter[0] = xRouter;
			xyNewRouter[1] = yRouter;

			// si la case n'est une cellule cible ou n'apporte pas de nouvelles cellule couvertes /!\ VOIR SI ON A LE DROIT DE METTRE UN ROUTEUR SUR UNE CELLULE VIDE
			if (aMap(xRouter, yRouter) != Cell::Wireless || nbCells == 0)
			{
				convolvedMat[maxValuesVect[random_integer]] = -kernelSize; // pour ne pas reprendre cette position dans la matrice convoluee
			}
			else findPosition = true;

		}

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
				kruskal(xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists

			}
			else routeurs.push_back(xyNewRouter);
		}
		else kruskal(xyNewRouter, routeurs, idx, idy, dists, succ, cost); // modifie cost, succ, routeurs, idx, idy et dists


		// si encore du budget
		if (succ)
		{
			// recuperation du masque du perimetre du routeur
			Matrix mask(2 * aRayonRouteurs + 1, 2 * aRayonRouteurs + 1);

			wirelessAccess(xyNewRouter[0], xyNewRouter[1], aMap, mask);

			int indexMask = 0;
			const int xFrom = xyNewRouter[0] - aRayonRouteurs;
			const int xTo = xyNewRouter[0] + aRayonRouteurs;
			const int yFrom = xyNewRouter[1] - aRayonRouteurs;
			const int yTo = xyNewRouter[1] + aRayonRouteurs;
			// dans la matrice targetCells, on met a 1 les cellules qui sont dans le perimetre du routeur, et on laisse a 0 les autres ou celles qui sont cachees pas un mur
			for (int xTargetCell = xFrom; xTargetCell <= xTo; xTargetCell++)
			{
				for (int yTargetCell = yFrom; yTargetCell <= yTo; yTargetCell++)
				{
					// maj de targetCellsForConvolution et de targetCells si nouvelle cellule pas encore couverte
					if (mask(indexMask) == 1 && targetCells(xTargetCell, yTargetCell) == 0)
					{
						targetCellsForConvolution(xTargetCell, yTargetCell) = -1;
						targetCells(xTargetCell, yTargetCell) = 1;
						nbTargetCells--;
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
			displayScore(targetCells, routeurs.size() - 1);
			return;

		}

		//targetCellsCoords.clear(); // on vide le vecteur, ce qui aura effet de delete les new int[2] (eviter les fuites memoires)
		//delete xyNewRouter;
	}
	// #################################################################################
	// peut etre a enlever pr les grosses map
	placeMstPaths(routeurs, idx, idy, dists);
	// #################################################################################
	std::cout << "  " << routeurs.size() - 1 << "\t\t\t|\t\t" << (useKruskal ? "" : "env. ") << cost << " / " << aBudgetOriginal << " = " << (cost * 100) / aBudgetOriginal << "%" << "\t\t|\t\t" << nbCellsCovered(targetCells) << " / " << getNbCellsOriginal() << " = " << cellsCoveredPercentage(targetCells) << "%";
	std::cout << '\r';
	displayScore(targetCells, routeurs.size() - 1);
	
}

void Algo::run(int bestScore)
{
	std::cout << "  Algo\t\t\t" << aMethod << "\n" << std::endl;
	aBestScore = bestScore;
	if (aMethod == "random")
	{
		random();
	}
	else if (aMethod == "bigcase")
	{
		bigCase();
	}
	else if (aMethod == "gaussian_blur")
	{
		gaussianBlur();
	}
}