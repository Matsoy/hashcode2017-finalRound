#include "arbitre.h"
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#include <atlstr.h>
#else
#include <dirent.h>
#endif


int testmain()
{
	//Changer les 3 parametres en fonction de la methode, de la map et du timestamp
	std::string method = "random";
	std::string map = "opera";
	std::string extension = "1512154692";

	checkSolution("../../solutions/" + map + "/" + method + "-" + map + "-" + extension + ".out");
	return(EXIT_SUCCESS);
}

/*
* code executable de l'arbitre. Test une map dont le nom est donne en outfile avec chaque solutions.
* renvoie score (0) si solution non conforme + temps moyen d'execution pour chaque.
* @param dir repertoire qui contient les executables des strategies elaborees.
* @param outfile nom du fichier de sorti qui est aussi nom de la map que l'on souhaite tester.
*/



int main(int argc, char* argv[]) {
	std::string dir = argv[1];
	std::cout << "un arg:" << dir << "\n";
	std::string outf = argv[2];
	std::cout << "deux arg: "<<outf<<"\n";
	std::string inputdir = "..\\..\\inputs\\";
	std::string outputdir = "..\\..\\solutions\\";
	std::vector<std::string> execlist;
	GetFilesInDirectory(execlist,dir,"");
	for (auto &s : execlist) {
		std::cout << s;
	}
	std::map<int, std::pair<float, std::string>> results;
	float sumtimes;
	for (auto &s : execlist) { //pour chaque strat
		int maxscore = 0;
		int testcount = 0;
		sumtimes = 0;
		size_t lastindex = s.find_last_of(".");
		std::string stratname = s.substr(0, lastindex);
		std::string outputdirmap = outputdir + outf;
		while (testcount <= 5) {
			clock_t t1, t2;
			
			std::string temp_concat = "..\\..\\executables\\"+stratname +" "+inputdir+outf+".in"+" "+outputdirmap+"\\"+outf+"-"+stratname+std::to_string(testcount)+".out";

			const char* fullcommand = temp_concat.c_str();  //commande a lancer
			std::cout << fullcommand << "\n";
			t1 = clock();
			system(fullcommand);  //test strat avec appel systeme
			t2 = clock();
			float secondsdiff = (float)t2 - (float)t1;
			sumtimes += secondsdiff;
			testcount += 1;
		}
		std::string soldir = "..\\..\\solutions\\" + outf;
		std::vector<std::string>solutions;
		GetFilesInDirectory(solutions, soldir,s);  //scan fichiers sortis
		for(auto &s2:solutions){

			int score = checkSolution(s2);  //verif solution valide
			if (score > maxscore) maxscore = score;
		}
		results[maxscore] = std::make_pair(sumtimes / 5, s);  //stockage resultats de chaque strat apres 5 tests.
		CleanupDirectory(outputdirmap);
	}
	std::string outffull = outf+".txt";
	std::ofstream outputS;
	outputS.open(outffull); //creation fichier out
	for (const auto& result : results) {
		std::cout << "executable :" << result.second.second << " temps moyen: " << result.second.first << " score: " << result.first << "\n";
	}
	return 0;
}

std::vector<int> infoMap(const std::string & mapName)
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


bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected)
{
	if (abs(xCable - xConnected) <= 1 && abs(yCable - yConnected) <= 1)
	{
		return true;
	}
	return false;
}


bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::vector<bool>>& cablesVector)
{
	for (int i = xCable - 1; i < xCable + 2; i++)
	{
		for (int j = yCable - 1; j < yCable + 2; j++)
		{
			if ((i != xCable || j != yCable) && cablesVector[i][j] == true)
			{
				return true;
			}
		}
	}
	return false;
}


std::vector<std::vector<int>> parseMap(const std::string & mapName, const int & row, const int & column)
{
	int coordX = 0;
	int coordY = 0;
	std::vector<std::vector<int>> mapMatrix(row, std::vector<int>(column));
	std::ifstream file(mapName);
	std::string line = "";	//ligne courante


							//Il faut passer les 3 premi鑽es lignes pour ensuite lire la map

	//Il faut passer les 3 premieres lignes pour ensuite lire la map

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
				mapMatrix[coordX][coordY] = Cell::Wall;
			}
			else if (charElement == '-')
			{
				mapMatrix[coordX][coordY] = Cell::Void;
			}
			else if (charElement == '.')
			{
				mapMatrix[coordX][coordY] = Cell::Wireless;
			}
			coordY += 1;
		}
		coordX += 1;
	}

	return mapMatrix;
}


int scoreRouter(std::vector<std::vector<int>>& map, const std::vector<std::pair<int, int>>& routerCoord, const int & routerRadius, const int & row, const int & column)
{
	int score = 0;

	for (auto it = routerCoord.begin(); it != routerCoord.end(); it++)
	{
		for (int i = (it->first - routerRadius); i <= (it->first + routerRadius); i++)
		{
			for (int j = (it->second - routerRadius); j <= (it->second + routerRadius); j++)
			{
				if (map[i][j] == Cell::Wireless && wallPresence(map, it->first, it->second, i, j) == false)
				{
					map[i][j] = Cell::Connected;
					score += 1000;
				}
			}
		}
	}
	return score;
}


bool wallPresence(const std::vector<std::vector<int>>& map, const int & xRouter, const int & yRouter, const int & xCell, const int & yCell)
{
	//Selon si le routeur est plus haut ou bas, plus ・gauche ou ・droite, alors il faudra aller dans un sens ou un autre.
	//C'est pour cela qu'il y a la condition d'arret et l'incrementation/decrementation avec les conditions ternaires.
	for (int i = xRouter; ((xRouter - xCell) < 0) ? i <= xCell : i >= xCell; ((xRouter - xCell) < 0) ? i++ : i--)
	{
		for (int j = yRouter; ((yRouter - yCell) < 0) ? j <= yCell : j >= yCell; ((yRouter - yCell) < 0) ? j++ : j--)
		{
			if (map[i][j] == Cell::Wall)
			{
				return true;
			}
		}
	}
	return false;
}


int checkSolution(const std::string & solutionFile)
{	
	
	std::string sdir = "..\\..\\solutions\\"+solutionFile.substr(0,solutionFile.find("-"))+"\\";
	std::cout << sdir+solutionFile + "\n";
	std::string mapFile = "../../inputs/" + solutionFile.substr(0,solutionFile.find("-")) + ".in";
	std::cout << "Test de la solution : " << solutionFile.substr(16) << std::endl;
	std::cout << "Sur la map : " << mapFile.substr(13) << std::endl << std::endl;

	std::vector<int> data = infoMap(mapFile);	//Contient les informations liees ・la map
	const int row = data[0];
	const int column = data[1];
	const int radius = data[2];
	const int cableCost = data[3];
	const int routerCost = data[4];
	const int budget = data[5];
	const int xBackbone = data[6];
	const int yBackbone = data[7];

	std::ifstream solution(sdir+solutionFile, std::ios::in);
	if (solution)	//On test si le fichier est bien ouvert
	{
		//Parser le fichier de solution
		int nbCable = 0;
		int nbRouters = 0;

		std::vector<std::vector<int>> map = parseMap(mapFile, row, column);
		std::vector<std::vector<bool>> cablesMatrix(row, std::vector<bool>(column));
		std::vector<std::pair<int, int>> routerCoord;
		//std::vector<std::vector<bool>> routerMatrix(row, std::vector<bool>(column));

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
						if (cableOrdered(a, b, cablesMatrix) || distInf1(a, b, xBackbone, yBackbone))
						{
							cablesMatrix[a][b] = true;
						}
						else
						{
							std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
							std::cout << "Probleme dans l'ordre des cables pour le cable de coordonnees (" << a << ',' << b << ')' << std::endl;
							return 0;
						}
					}
					else
					{
						std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
						std::cout << "Coordonnees hors limite pour le cable de coordonnees (" << a << ',' << b << ')' << std::endl;
						return 0;
					}
				}
				else
				{
					std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
					std::cout << "Mauvaise forme des coordonnees pour le cable en position " << i << std::endl;
					return 0;
				}
			}

			std::getline(solution, line);	//Pour avoir le nombre de routeurs
			nbRouters = stoi(line);

			if ((nbCable * cableCost + nbRouters * routerCost) > budget)	//Test du budget
			{
				std::cout << "Le budget a ete depasse";
				return 0;
			}

			if (0 <= nbRouters && nbRouters < row*column)
			{
				int a, b;
				for (int i = 0; i < nbRouters; i++)
				{
					std::getline(solution, line);
					std::istringstream iss(line);
					if (iss >> a >> b)
					{
						if (0 <= a && 0 <= b && a < row && b < column)
						{
							if (cablesMatrix[a][b] == true && map[a][b] != Cell::Wall)
							{
								routerCoord.push_back(std::pair<int, int>(a, b));	//Ajout・car pas sur un mur et cabl・
							}
							else
							{
								std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
								std::cout << "Le routeur (" << a << ',' << b << ") est sur un mur ou n'est pas cable" << std::endl;
								return 0;
							}
						}
						else
						{
							std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
							std::cout << "Coordonnes hors limite pour le routeur de coordonnes (" << a << ',' << b << ')' << std::endl;
							return 0;
						}
					}
					else
					{
						std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
						std::cout << "Mauvaise forme des coordonnees pour le routeur en position " << i << std::endl;
						return 0;
					}
				}
			}
			else
			{
				std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
				std::cout << "Il y a plus de routeurs que de cases sur la map" << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "La solution proposee n'est pas valide pour la raison suivante : " << std::endl;
			std::cout << "Il y a plus de cables que de cases sur la map" << std::endl;
			return 0;
		}

		std::cout << "La solution est valide" << std::endl;
		solution.close();

		int score = scoreRouter(map, routerCoord, radius, row, column) + budget - (nbCable * cableCost + nbRouters * routerCost);
		std::cout << "Score : " << score << std::endl;
		return score;
	}
	else
	{
		std::cout << "File of the solution can not be opened" << std::endl;
		return 0;
	}
}
std::string remove_extension(const std::string& filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}



void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, const std::string filter)
{
#ifdef WIN32
	HANDLE dir;
	WIN32_FIND_DATAA file_data;
	std::string  file_name;
	if ((dir = FindFirstFileA((directory + "/*.*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
	{
		std::cout << "invalid dir";
	}
	
	while (FindNextFileA(dir, &file_data)) {
		file_name = file_data.cFileName;
		if (strcmp(file_data.cFileName, ".") != 0 && strcmp(file_data.cFileName, "..") != 0)
		{
			if (filter == "") {
				out.push_back(file_name);
			}
			else {
				if (file_name.find(filter)) {
					out.push_back(file_name);
				}
			}
		}
	}
	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory.c_str());
	while ((ent = readdir(dir)) != NULL) {
		const std::string file_name = ent->d_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;
		if (filter == "") {
			out.push_back(file_name);
		}
		else {
			if (file_name.find(filter)) {
				out.push_back(file_name);
			}
		}
	}
	closedir(dir);
#endif
}

void CleanupDirectory(const std::string & directory)
{
#ifdef WIN32
	HANDLE dir;
	WIN32_FIND_DATAA file_data;
	std::string  file_name;
	if ((dir = FindFirstFileA((directory + "/*.*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
	{
		std::cout << "invalid dir";
	}

	while (FindNextFileA(dir, &file_data)) {
		remove(file_data.cFileName);
	}
	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory.c_str());
	while ((ent = readdir(dir)) != NULL) {
		const std::string file_name = ent->d_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;
		remove(file_name);
	}
	closedir(dir);
#endif
}
