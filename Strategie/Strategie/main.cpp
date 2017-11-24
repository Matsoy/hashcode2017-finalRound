#include "Header.h"
#include "Matrix.h"

int* initializeData(std::string fichierInput) {
	int retDatas[8] = {};

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
				if (wordIndex == 1)	retDatas[6] = std::stoi(word); // coordonnee X de l'antenne --> = index ligne
				else retDatas[7] = std::stoi(word); // coordonnee Y de l'antenne --> = index colonne

				wordIndex++;
			}
		}
		else break; //sinon, c'est la carte

		wordIndex = 1;
		lineIndex++;
	}

	return retDatas;
}


int main()
{
	std::string fichierInput = "../../inputs/simple_example.in";

	int* datas = initializeData(fichierInput); // tableau de la forme [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]

											   // initialisation des donnees constantes
	int nbLignes = datas[0];
	int nbColonnes = datas[1];
	int rayonRouteurs = datas[2];
	int prixCable = datas[3];
	int prixRouteur = datas[4];
	int budgetMax = datas[5];
	int xBackbone = datas[6];
	int yBackbone = datas[7];

	// initialisation de la matrice
	//Matrix<nbLignes, nbColonnes> map;// = initializeMap(fichierInput);

									 //Matrix<2, 3> map = { 1,2,3,4,5,6 };	//Servira pour produit matriciel; // matrice qui est une simple representation immuable de la carte (contient des objets Void, Wall, Celle et Backbone)
									 //Matrix<2, 3> solution = { 1,2,3,4,5,6 }; // matrice de meme dimension que map sur laquelle nous representons les routeurs et les cables (contient des objets Router et Wire)

}

/*
Matrix<int,int> & initializeMap(std::string fichierInput) {
std::ifstream input("../../inputs/simple_example.in");
std::string line; //ligne courante
unsigned int lineIndex = 1; //numero de la ligne
while (getline(input, line)) //pour chaque ligne
{

std::istringstream iss(line);
std::string word; //mot courant
unsigned int wordIndex = 1; //numero du mot

if (lineIndex == 1) //1ere ligne: nb de lignes / nombre de colonnes / rayon des routeurs
{
while (iss >> word) //pour chaque mot
{
if (wordIndex == 1)	nbLignes = std::stoi(word); // nombre de lignes
else if (wordIndex == 2) nbColonnes = std::stoi(word); // nombre de colonnes
else rayonRouteurs = std::stoi(word); // rayon des routeurs

wordIndex++;
}
}
else if (lineIndex == 2) //2eme ligne: prix d'un cable / prix d'un routeur / budget maximum
{
while (iss >> word) //pour chaque mot
{
if (wordIndex == 1)	prixCable = std::stoi(word); // prix d'un cable
else if (wordIndex == 2) prixRouteur = std::stoi(word); // prix d'un routeur
else budgetMax = std::stoi(word); // budget maximum

wordIndex++;
}
}
else if (lineIndex == 3) //3eme ligne: coordonnee X de l'antenne / coordonnee Y de l'antenne
{
while (iss >> word) //pour chaque mot
{
int xAntenne, yAntenne;

if (wordIndex == 1)	xAntenne = std::stoi(word); // coordonnee X de l'antenne --> = index ligne
else yAntenne = stoi(word); // coordonnee Y de l'antenne --> = index colonne

wordIndex++;
}
}
else //sinon, c'est la carte
{
// initialisation de la matrice d'elements

while (iss >> word) //pour chaque element de la carte
{

for (char& caracElement : word) {
switch (caracElement) {
case '-': // une cellule vide

break;
case '.': // cellule cible
break;
case '#': // mur
break;
default:
throw std::string("Caractere inconnu");
}
}

wordIndex++;
}
}

wordIndex = 1;
lineIndex++;
}
}

*/