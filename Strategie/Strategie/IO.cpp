#include "IO.h"

/*
* Constructeur
*/
IO::IO()
{
}

/*
* renvoie un tableau avec les donnees des 3 1eres lignes du fichier d'input
*
*@param inputFile le nom du fichier d'input a lire
*@return [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]
*/
int* IO::initializeData(std::string inputFile)
{
	int * retDatas = new int[8];

	std::ifstream input(inputFile);
	std::string line; //ligne courante
	unsigned int lineIndex = 1; //numero de la ligne

	while (std::getline(input, line)) //pour chaque ligne
	{
		std::istringstream iss(line);
		std::string word; //mot courant
		unsigned int wordIndex = 1; //numero du mot

		if (lineIndex == 1) //1ere ligne: nb de lignes / nombre de colonnes / rayon des routeurs
		{
			while (iss >> word) //pour chaque mot
			{
				if (wordIndex == 1)
				{
					retDatas[0] = std::stoi(word); // nombre de lignes
				}
				else if (wordIndex == 2)
				{
					retDatas[1] = std::stoi(word);// nombre de colonnes
				}
				else
				{
					retDatas[2] = std::stoi(word); // rayon des routeurs
				}
				wordIndex++;
			}
		}
		else if (lineIndex == 2) //2eme ligne: prix d'un cable / prix d'un routeur / budget maximum
		{
			while (iss >> word) //pour chaque mot
			{
				if (wordIndex == 1)
				{
					retDatas[3] = std::stoi(word); // prix d'un cable
				}
				else if (wordIndex == 2)
				{
					retDatas[4] = std::stoi(word); // prix d'un routeur
				}
				else
				{
					retDatas[5] = std::stoi(word); // budget maximum
				}

				wordIndex++;
			}
		}
		else if (lineIndex == 3) //3eme ligne: coordonnee X de l'antenne / coordonnee Y de l'antenne
		{
			while (iss >> word) //pour chaque mot
			{
				if (wordIndex == 1)
				{
					retDatas[6] = std::stoi(word); // coordonnee X de l'antenne --> = index ligne
					backboneX = retDatas[6];
				}
				else
				{
					retDatas[7] = std::stoi(word); // coordonnee Y de l'antenne --> = index colonne
					backboneY = retDatas[7];
				}
				wordIndex++;
			}
		}
		else {
			break; //sinon, c'est la carte
		}

		wordIndex = 1;
		lineIndex++;
	}

	return retDatas;
}



/*
* Rempli la matrice d'entiers
* - -> -1
* # -> 0
* . -> 1
*
*@param la reference de la matrice
*@param inputFile le nom du fichier d'input a lire
*/
void IO::initializeMap(Matrix & m, std::string inputFile)
{
	std::ifstream input(inputFile);
	std::string line; //ligne courante
	unsigned int lineIndex = 1; //numero de la ligne
	while (getline(input, line)) //pour chaque ligne
	{
		std::istringstream iss(line);

		if (lineIndex > 3) // si on arrive a la carte
		{
			// initialisation de la matrice
			for (unsigned int i = 0; i<line.length(); i++)
			{
				switch (line.at(i)) {
				case '-': // une cellule vide
					m(lineIndex - 4, i) = Cell::Void; // -1
					break;
				case '#': // mur
					m(lineIndex - 4, i) = Cell::Wall; // 0
					break;
				default: // sinon cellule cible
					m(lineIndex - 4, i) = Cell::Wireless; // 1
					break;
				}
			}
		}
		lineIndex++;
	}
}


/*
* Rempli la matrice d'entiers en fonction des routeurs d'un fichier solution
* routeurs -> 3
*
*@param la reference de la matrice
*@param solutionFile le nom du fichier solution a lire
*/
int IO::initializeMapFromSolution(Matrix & m, std::string solutionFile, std::vector<int *> & routeurs)
{
	routeurs.push_back(new int[2]{ backboneX, backboneY });
	int nbRouteurs = 0;

	std::ifstream input(solutionFile);
	std::string line; //ligne courante
	int lineIndex = 1; //numero de la ligne

	int nbCables = 0;

	while (getline(input, line)) //pour chaque ligne
	{
		std::istringstream iss(line);
		std::string word; //mot courant

		

		if (lineIndex == 1)
		{
			nbCables = atoi(line.c_str());
		}
		else if (lineIndex == nbCables + 2) // nb de routeurs
		{
			nbRouteurs = atoi(line.c_str());

		}
		else if (lineIndex > (nbCables + 2)) // les coords des routeurs
		{
			int xRout, yRout;
			int wordIndex = 1;

			while (iss >> word) //pour chaque mot
			{
				if (wordIndex == 1)
				{
					xRout = std::stoi(word); // nombre de lignes
				}
				else if (wordIndex == 2)
				{
					yRout = std::stoi(word);// nombre de colonnes
				}
				wordIndex++;
			}

			m(xRout, yRout) = Cell::ConnectedRouter;
			routeurs.push_back(new int[2]{ xRout, yRout });
		}
		
		lineIndex++;
	}

	return nbCables;

}

/*
* Genere le fichier output, en provocant le premier appel de la methode recursive ioBrowser et enregistre le resultat dans un fichier donne en parametre.
*
* @param mapRouteurs la matrice contenant les routeurs et les cables
* @param pathBeginning le debut du nom du fichier d'output qui va etre genere
*/
void IO::generateOutput(Matrix & mapRouteurs, std::string pathBeginning)
{
	ioBrowser(mapRouteurs, backboneX, backboneY);
	std::string out = std::to_string(counterConnectedCells) + "\n";
	out.append(connectedcelltxt);
	out.append(std::to_string(counterRouter) + "\n");

	out.append(routeurtxt);

	//##################################
	//###  Enregistrement du fichier ###
	//##################################
	// timestamp
	time_t  timev;
	time(&timev);
	std::stringstream timess;
	timess << timev;
	std::string filePath = pathBeginning;
	//Ecrase le fichier s'il existe
	std::remove(filePath.c_str());
	//Creer le fichier
	std::ofstream write(filePath);
	//Insere les donnees
	write << out;
	//on ferme le fichier
	write.close();
	//##################################
	//##################################

	//std::cout << std::endl;
	//std::cout << "  " << filePath << "\n" << std::endl;
}

/*
* Parcours les chemins cables de maniere recursive pour retenir l'ordre et l'emplacement des cables et routeurs
*
* @param mapRouteurs la matrice avec les routeurs et les cables
* @param x coordonnee x du cable ou du routeur
* @param y coordonnee y du cable ou du routeur
*/
void IO::ioBrowser(Matrix & mapRouteurs, int x, int y)
{
	//Si c'est une case valide. i.e. un routeur, un cable ou le backbone
	if (mapRouteurs(x, y) == 3 || mapRouteurs(x, y) == 4 || (x == backboneX && y == backboneY))
	{
		//Si c'est un routeur cable
		if (mapRouteurs(x, y) == 3)
		{
			counterRouter++;
			routeurtxt.append(std::to_string(x) + " " + std::to_string(y) + "\n");

			//Si le routeur n'est pas sur le backbone
			if (x != backboneX || y != backboneY)
			{
				counterConnectedCells++;
				connectedcelltxt.append(std::to_string(x) + " " + std::to_string(y) + "\n");
			}
		}
		//Si c'est un cable
		else if (mapRouteurs(x, y) == 4)
		{
			counterConnectedCells++;
			connectedcelltxt.append(std::to_string(x) + " " + std::to_string(y) + "\n");
		}

		//on efface la cellule et on regarde ensuite les cellules voisines
		mapRouteurs(x, y) = -9;
		ioBrowser(mapRouteurs, x - 1, y);
		ioBrowser(mapRouteurs, x + 1, y);
		ioBrowser(mapRouteurs, x, y - 1);
		ioBrowser(mapRouteurs, x, y + 1);
		ioBrowser(mapRouteurs, x - 1, y - 1);
		ioBrowser(mapRouteurs, x - 1, y + 1);
		ioBrowser(mapRouteurs, x + 1, y - 1);
		ioBrowser(mapRouteurs, x + 1, y + 1);
	}
}

/*
* Check si le fichier existe
*
* @param le chemin du fichier
* @return true si le fichier existe
*			false sinon
*/
bool IO::isFileExist(std::string filePath)
{
	std::ifstream infile(filePath);
	return infile.good();
}


