#include "IO.h"

IO::IO()
{
}

/*
* renvoie un tableau avec les donnees des 3 1eres lignes du fichier d'input
*
*@param fichierInput le nom du fichier d'input a lire
*@return [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]
*/
int* IO::initializeData(std::string fichierInput)
{
	int * retDatas = new int[8];

	std::ifstream input(fichierInput);
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
				if (wordIndex == 1)	retDatas[0] = std::stoi(word); // nombre de lignes
				else if (wordIndex == 2) retDatas[1] = std::stoi(word); // nombre de colonnes
				else retDatas[2] = std::stoi(word); // rayon des routeurs

				wordIndex++;
			}
		}
		else if (lineIndex == 2) //2eme ligne: prix d'un cable / prix d'un routeur / budget maximum
		{
			while (iss >> word) //pour chaque mot
			{
				if (wordIndex == 1)	retDatas[3] = std::stoi(word); // prix d'un cable
				else if (wordIndex == 2) retDatas[4] = std::stoi(word); // prix d'un routeur
				else retDatas[5] = std::stoi(word); // budget maximum

				wordIndex++;
			}
		}
		else if (lineIndex == 3) //3eme ligne: coordonnee X de l'antenne / coordonnee Y de l'antenne
		{
			while (iss >> word) //pour chaque mot
			{
				if (wordIndex == 1) {
					retDatas[6] = std::stoi(word); // coordonnee X de l'antenne --> = index ligne
					backboneX = retDatas[6];
				}
				else {
					retDatas[7] = std::stoi(word); // coordonnee Y de l'antenne --> = index colonne
					backboneY = retDatas[7];
				}

				wordIndex++;
			}
		}
		else break; //sinon, c'est la carte

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
*@param fichierInput le nom du fichier d'input a lire
*/
void IO::initializeMap(Matrix & m, std::string fichierInput)
{
	int cpt = 0;

	std::ifstream input(fichierInput);
	std::string line; //ligne courante
	unsigned int lineIndex = 1; //numero de la ligne
	while (getline(input, line)) //pour chaque ligne
	{
		std::istringstream iss(line);

		if (lineIndex > 3) // si on arrive à la carte
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
*Génère le fichier output, en provocant le premier appel de la méthode récursive ioBrowser
*et enregistre le résultat dans un fichier donné en paramètre.
*/
void IO::generateOutput(Matrix & mapRouteurs, std::string file)
{
	
	ioBrowser(mapRouteurs, backboneX, backboneY);
	std::string out = std::to_string(counterConnectedCells)+"\n";
	out.append(connectedcelltxt);
	out.append(std::to_string(counterRouter) + "\n");
	out.append(routeurtxt);
	std::cout << out << std::endl;

	/*

	//##################################
	//###  Enregistrement du fichier ###
	//##################################

	//Ecrase le fichier s'il existe
	std::remove(file.c_str());
	//Créer le fichier
	std::ofstream write(file);
	//Insère les données
	write << out;
	//on ferme le fichier
	write.close();

	//##################################
	//##################################


	*/
}

/*
* Parcours les chemins cablés de manière récursive pour retenir l'ordre et l'emplacement des cables et routeurs
*/
void IO::ioBrowser(Matrix & mapRouteurs, int x, int y)
{
	//Si c'est une case valide
	if (mapRouteurs(x, y) == 3 || mapRouteurs(x, y) == 4 || mapRouteurs(x, y) == -2) {
		//Si c'est un routeur cablé
		if (mapRouteurs(x, y) == 3) {
			counterConnectedCells++;
			counterRouter++;
			connectedcelltxt.append(std::to_string(x) + " " + std::to_string(y)+"\n");
			routeurtxt.append(std::to_string(x) + " " + std::to_string(y) + "\n");
		}
		//Si c'est un cable
		else if (mapRouteurs(x, y) == 4) {
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


