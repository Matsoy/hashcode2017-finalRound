#pragma once
#include "Header.h"
#include "Matrix.h"
#include "Cell.h"

/*
* Classe qui gere les entrees / sorties
*/
class IO
{

public:
	/*
	* Constructeur
	*/
	IO();
	/*
	* renvoie un tableau avec les donnees des 3 1eres lignes du fichier d'input
	*
	*@param inputFile le nom du fichier d'input a lire
	*@return [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]
	*/
	int* initializeData(std::string fichierInput);
	/*
	* Rempli la matrice d'entiers
	* - -> -1
	* # -> 0
	* . -> 1
	*
	*@param la reference de la matrice
	*@param inputFile le nom du fichier d'input a lire
	*/
	void initializeMap(Matrix & m, std::string fichierInput);
	/*
	* Rempli la matrice d'entiers en fonction des routeurs d'un fichier solution
	* routeurs -> 3
	*
	*@param la reference de la matrice
	*@param solutionFile le nom du fichier solution a lire
	*/
	int initializeMapFromSolution(Matrix & m, std::string solutionFile, std::vector<int *> & routeurs);
	/*
	* Genere le fichier output, en provocant le premier appel de la methode recursive ioBrowser et enregistre le resultat dans un fichier donne en parametre.
	*
	* @param mapRouteurs la matrice contenant les routeurs et les cables
	* @param pathBeginning le debut du nom du fichier d'output qui va etre genere
	*/void generateOutput(Matrix & mapRouteurs, std::string file);
	/*
	* Check si le fichier existe
	*
	* @param le chemin du fichier
	* @return true si le fichier existe
	*			false sinon
	*/
	bool isFileExist(std::string filePath);

private:
	int counterRouter = 0; // nombre de routeurs comptes
	int counterConnectedCells = 0; // nombre de cellules connectees comptess
	int backboneX; // coordonnee x du backbone (emetteur)
	int backboneY; // coordonnee y du backbone (emetteur)
	std::string routeurtxt = ""; // texte contenant la partie sur le positionnement des routeurs 
	std::string connectedcelltxt = ""; // texte contenant la partie sur le positionnement des cellules connectees 

	/*
	* Parcours les chemins cables de maniere recursive pour retenir l'ordre et l'emplacement des cables et routeurs
	*
	* @param mapRouteurs la matrice avec les routeurs et les cables
	* @param x coordonnee x du cable ou du routeur
	* @param y coordonnee y du cable ou du routeur
	*/
	void ioBrowser(Matrix & mapRouteurs, int x, int y);
};