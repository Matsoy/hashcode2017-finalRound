//Ajout de la solution
#include "arbitre.h"

int main()
{
	checkTest("../../solutions/solutionTestPasOk.txt");
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


//bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::pair<int, int>>& cableVector)
//{
//	for (auto it = cableVector.begin(); it != cableVector.end(); it++)
//	{
//		if (distInf1(xCable, yCable, it->first, it->second))
//		{
//			return true;
//		}
//	}
//	return false;
//}

bool cableOrdered2(const int & xCable, const int & yCable, const std::map<int, std::vector<int>>& cablesMap)
{
	for (auto it1 = cablesMap.begin(); it1 != cablesMap.end(); it1++)
	{
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
		{
			if (distInf1(xCable, yCable, it1->first, (*it2)))
			{
				return true;
			}
		}
	}
	return false;
}


bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected)
{
	if (abs(xCable - xConnected) <= 1 && abs(yCable - yConnected) <= 1)
	{
		return true;
	}
	return false;
}


bool checkTest(const char* solutionName)	//Prévoir un second param pour le fichier .in
{
	std::vector<int> data = infoMap("../../inputs/simple_example.in");	//Contient les informations liées à la map
	const int row = data[0];
	const int column = data[1];
	const int radius = data[2];
	const int cableCost = data[3];
	const int routerCost = data[4];
	const int budget = data[5];
	const int xBackbone = data[6];
	const int yBackbone = data[7];
	std::map<int, std::vector<int>> murs = detectMurs("../../inputs/simple_example.in");
	
	std::ifstream solution(solutionName, std::ios::in);
	if (solution)	//On test si le fichier est bien ouvert
	{
		//Parser le fichier de solution
		int nbCable = 0;
		int nbRouteurs = 0;
		//std::vector<std::pair<int, int>> cableCoord;
		std::map<int, std::vector<int>> cables;
		std::vector<std::pair<int, int>> routeurCoord;
		bool cable = false;


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
					if (0 <= a && 0 <= b && a < row && b < column)
					{
						if (cableOrdered2(a, b, cables) || distInf1(a, b, xBackbone, yBackbone))
						{
							if (cables.find(a) == cables.end())
							{
								cables.emplace(a, std::vector<int>());
							}
							cables.at(a).push_back(b);
						}
						else
						{
							//Pas de cables connectés autour, problème d'ordre
							return false;
						}
					}
					else
					{
						//Il y a un problème avec les coordonnées des cables (valeurs)
						return false;
					}

				}
				else
				{
					//Il y a un problème avec les coordonnées des cables (forme)
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
							if (murs.find(a) != murs.end())	//Il y a des murs sur la ligne
							{
								for (auto it = murs.at(a).begin(); it != murs.at(a).end(); it++)
								{
									//On regarde si le routeur est sur un mur
									if (b == (*it))
									{
										//Routeur sur un mur (de coordonnées (a,b))
										return false;
									}
								}
							}
							else if (cables.find(a) != cables.end())
							{
								cable = false;
								for (auto it = cables.at(a).begin(); it != cables.at(a).end(); it++)
								{
									if ((*it) == b)
									{
										cable = true;
									}
								}
							}
							if (cable == true)
							{
								cable = false;
								routeurCoord.push_back(std::pair<int, int>(a, b));	//Ajouté car pas sur un mur et cablé
							}
							else
							{
								//Problème : la case du routeur n'est pas câblée, routeur pas connecté au backbone
								return false;
							}
						}
						else
						{
							//Il y a un problème avec les coordonnées des routeurs (valeurs)
							return false;
						}
					}
					else
					{
						//Il y a un problème avec les coordonnées des routeurs (formes)
						return false;
					}
				}
			}
		}

		else
		{
			//Il y a trop de cables
			return false;
		}

		//Ici, on a les coordonnées des cables et des routeurs dans les vecteurs
		std::cout << "On est bon" << std::endl;
		solution.close();
		return true;
	}
	else
	{
		std::cerr << "File of the solution can not be opened" << std::endl;
		return false;	//Faire exception dans le futur
	}
}
