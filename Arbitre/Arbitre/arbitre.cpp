//Ajout de la solution
#include "arbitre.h"

int main()
{
	checkTest();
	//checkSolution("MettreCheminFichier");
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

bool checkSolution(const char* solutionName)
{
	std::vector<int> data = infoMap("../../inputs/simple_example.in");	//Contient les informations liées à la map
	std::ifstream solution(solutionName, std::ios::in);
	if (solution)	//On test si le fichier est bien ouvert
	{

		solution.close();
	}
	else
	{
		std::cerr << "File of the solution can not be opened" << std::endl;
	}
	return false;
}

bool checkTest()
{
	std::vector<int> data = infoMap("../../inputs/simple_example.in");	//Contient les informations liées à la map
	for (std::vector<int>::iterator it = data.begin(); it != data.end(); it++)
	{
		std::cout << "Données :" << (*it) << std::endl;
	}
	return false;
}
