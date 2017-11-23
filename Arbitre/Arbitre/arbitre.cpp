//Ajout de la solution
#include "arbitre.h"

int main()
{
	checkTest("../../solutions/solutionTestOk.txt");
	//checkTest("../../solutions/solutionTestPasOk.txt");
	return(EXIT_SUCCESS);
}

std::vector<int> infoMap(const char * mapName)
{
	std::vector<int> data;
	std::ifstream file(mapName);
	std::string line = "";	//ligne courante
	int word = 0;	//mot courrant

	for (int i = 1; i < 4; i++)
	{
		getline(file, line);
		std::istringstream iss(line);
		while (iss >> word)
		{
			data.push_back(word);
		}
	}
	return data;
}


std::map<int, std::vector<int>> detectMurs(const char * mapName)
{
	int coordX = 0;
	int coordY;
	std::map<int, std::vector<int>> murs;

	std::ifstream file(mapName);
	std::string line = "";	//ligne courante

	//Il faut passer les 3 premières lignes pour ensuite lire la map
	for (int i = 1; i < 4; i++)
	{
		getline(file, line);
	}
	while (getline(file, line))
	{
		coordY = 0;
		for (const char& charElement : line)
		{
			if (charElement == '#')
			{
				if (murs.find(coordX) == murs.end())	//Si on n'avait pas de murs sur la ligne, il faut "créer" le vecteur
				{
					murs.emplace(coordX, std::vector<int>());
				}
				murs.at(coordX).push_back(coordY);
			}
			coordY += 1;
		}
		coordX += 1;
	}

	return murs;
}


bool checkTest(const char* solutionName)	//Prévoir un second param pour le fichier .in
{
	std::vector<int> data = infoMap("../../inputs/simple_example.in");	//Contient les informations liées à la map
	int row = data[0];
	int column = data[1];
	int radius = data[2];
	int cableCost = data[3];
	int routerCost = data[4];
	int budget = data[5];
	int xBackbone = data[6];
	int yBackbone = data[7];
	std::map<int, std::vector<int>> murs = detectMurs("../../inputs/simple_example.in");
	
	std::ifstream solution(solutionName, std::ios::in);
	if (solution)	//On test si le fichier est bien ouvert
	{
		//Parser le fichier de solution
		int nbCable = 0;
		int nbRouteurs = 0;
		std::vector<std::pair<int, int>> cableCoord;
		std::vector<std::pair<int, int>> routeurCoord;

		std::string line = "";
		std::getline(solution, line);
		nbCable = stoi(line);

		if (0 <= nbCable && nbCable < row*column)
		{
			int a, b;
			for (int i = 0; i < nbCable; i++)
			{
				std::getline(solution, line);
				std::istringstream iss(line);

				if (iss >> a >> b)
				{
					//Prévoir les tests de 8 voisinnage (cellules en ordre)

					if (0 <= a && 0 <= b && a < row && b < column)
					{
						cableCoord.push_back(std::pair<int, int>(a, b));
					}
					else
					{
						//Il y a un problème avec les coordonnées des routeurs
						return false;
					}

				}
				else
				{
					//Il y a un problème avec les coordonnées des cables
					return false;
				}
			}
			
			std::getline(solution, line);
			nbRouteurs = stoi(line);

			if ((nbCable * cableCost + nbRouteurs * routerCost) > budget)	//Test du budget
			//On test le budget maintenant, car si pas bon, cela évite de lire la suite du fichier pour rien
			{
				return false;
			}

			if (0 <= nbRouteurs && nbRouteurs < row*column)
			{
				int a, b;
				for (int i = 0; i < nbRouteurs; i++)
				{
					std::getline(solution, line);
					std::istringstream iss(line);
					if (iss >> a >> b)
					{
						if (0 <= a && 0 <= b && a < row && b < column)
						{
							if (murs.find(a) != murs.end())
							{
								for (auto it = murs.at(a).begin(); it != murs.at(a).end(); it++)
								{
									//On regarde si le routeur est sur un mur
									if (b == (*it))
									{
										return false;
									}
								}
							}
							routeurCoord.push_back(std::pair<int, int>(a, b));	//Ajouté car pas sur un mur
						}
						else
						{
							//Il y a un problème avec les coordonnées des routeurs
							return false;
						}
					}
					else
					{
						//Il y a un problème avec les coordonnées des routeurs
						return false;
					}
				}
			}
		}

		else
		{
			return false;	//Solution pas bonne
		}

		//Ici, on a les coordonnées des cables et des routeurs dans les vecteurs
		std::cout << "on est bon pour l'instant" << std::endl;

		solution.close();
		return true;
	}
	else
	{
		std::cerr << "File of the solution can not be opened" << std::endl;
		return false;	//Faire exception dans le futur
	}
}
