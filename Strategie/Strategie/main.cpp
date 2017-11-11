#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int main()
{

	cout << "Dans le main" << endl;


	ifstream input("../../inputs/simple_example.in");
	string line; //ligne courante
	unsigned int lineIndex = 1; //numéro de la ligne
	while (getline(input, line)) //pour chaque ligne
	{

		istringstream iss(line);

		if (lineIndex == 1) //1ere ligne: nb de lignes / nombre de colonnes / rayon des routeurs
		{
			string word; //mot courant
			unsigned int wordIndex = 1; //numéro du mot

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
			string word; //mot courant
			unsigned int wordIndex = 1; //numéro du mot

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
			string word; //mot courant
			unsigned int wordIndex = 1; //numéro du mot

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