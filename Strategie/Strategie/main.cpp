#include "Header.h"
#include "Matrix.h"
#include "Backbone.h"

using namespace std;

int main()
{

	int nbLignes;
	int nbColonnes;
	int rayonRouteurs;
	int prixCable;
	int prixRouteur;
	int budgetMax;
	Backbone backbone;
	//Matrix<2, 3> map = { 1,2,3,4,5,6 };	//Servira pour produit matriciel; // matrice qui est une simple représentation immuable de la carte (contient des objets Void, Wall, Celle et Backbone)
	//Matrix<2, 3> solution = { 1,2,3,4,5,6 }; // matrice de meme dimension que map sur laquelle nous representons les routeurs et les cables (contient des objets Router et Wire)

	ifstream input("../../inputs/simple_example.in");
	string line; //ligne courante
	unsigned int lineIndex = 1; //numéro de la ligne
	while (getline(input, line)) //pour chaque ligne
	{

		istringstream iss(line);
		string word; //mot courant
		unsigned int wordIndex = 1; //numéro du mot

		if (lineIndex == 1) //1ere ligne: nb de lignes / nombre de colonnes / rayon des routeurs
		{
			wordIndex = 1;
			while (iss >> word) //pour chaque mot
			{

				if (wordIndex == 1)
				{
					cout << "Nombre de lignes: " << word << endl;

				}
				else if (wordIndex == 2)
				{
					cout << "Nombre de colonnes: " << word << endl;
				}
				else
				{
					cout << "Rayon des routeurs: " << word << endl;
				}

				wordIndex++;
			}
		}
		else if (lineIndex == 2) //2eme ligne: prix d'un cable / prix d'un routeur / budget maximum
		{
			wordIndex = 1;

			while (iss >> word) //pour chaque mot
			{

				if (wordIndex == 1)
				{
					cout << "Prix d'un cable: " << word << endl;
				}
				else if (wordIndex == 2)
				{
					cout << "Prix d'un routeur: " << word << endl;
				}
				else
				{
					cout << "Budget maximum: " << word << endl;
				}

				wordIndex++;
			}
		}
		else if (lineIndex == 3) //3eme ligne: coordonnée X de l'antenne / coordonnée Y de l'antenne
		{
			wordIndex = 1;

			while (iss >> word) //pour chaque mot
			{

				if (wordIndex == 1)
				{
					cout << "Coordonnee X de l'antenne: " << word << endl;
				}
				else
				{
					cout << "Coordonnee Y de l'antenne: " << word << endl;
				}

				wordIndex++;
			}
		}
		else //sinon, c'est la carte
		{
			cout << line << endl;
		}

		lineIndex++;
	}
}