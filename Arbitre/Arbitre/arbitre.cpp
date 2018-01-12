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
	std::cout << "test";

	std::vector<std::string> execlist;
	GetFilesInDirectory(execlist,dir,"");
	std::map<int, std::pair<float, std::string>> results;
	float sumtimes;
	for (auto &s : execlist) { //pour chaque strat
		std::cout << s << "\n";
		int maxscore = 0;
		int testcount = 0;
		sumtimes = 0;
		while (testcount <= 5) {
			clock_t t1, t2;
			std::string temp_concat = s +" "+ outf+" "+s+"-"+outf;
			const char* fullcommand = temp_concat.c_str();  //commande a lancer
			std::cout << fullcommand << "\n";
			t1 = clock();
			system(fullcommand);  //test strat avec appel systeme
			t2 = clock();
			float secondsdiff = (float)t2 - (float)t1;
			sumtimes += secondsdiff;
		}
		std::string soldir = "../../solutions/" + outf;
		std::vector<std::string>solutions;
		GetFilesInDirectory(solutions, soldir,s);  //scan fichiers sortis
		for(auto &s2:solutions){
			int score = checkSolution(s2);  //verif solution valide
			if (score > maxscore) maxscore = score;
		}
		results[maxscore] = std::make_pair(sumtimes / 5, s);  //stockage resultats de chaque strat apres 5 tests.
	}
	std::string outffull = outf+".txt";
	std::ofstream outputS;
	outputS.open(outffull); //creation fichier out
	for (const auto& result : results) {
		std::cout << "executable :" << result.second.second << " temps moyen: " << result.second.first << " score: " << result.first << "\n";
	}
	return 0;
}

//Cette fonction va permettre de tirer les informations sur la map (contenues dans les 3 premieres lignes)
std::vector<int> infoMap(const std::string & mapName)
{
	std::vector<int> data;	//Le vecteur qui sera renvoyee
	std::ifstream file(mapName);	//La map (.in)
	std::string line = "";	//ligne courante
	int word = 0;	//mot courrant

	for (int i = 1; i < 4; i++)
	{
		getline(file, line);
		std::istringstream iss(line);	//Va permettre de parser la ligne
		while (iss >> word)
		{
			data.push_back(word);	//On met le "mot" (qui est un nombre) dans le vector
		}
	}
	return data;
}

//Cette fonction va permette de determiner si deux points sont a une distance superieure
//ou inferieure ou egale ‡ 1 selon la norme infinie
bool distInf1(const int & xCable, const int & yCable, const int & xConnected, const int & yConnected)
{
	if (abs(xCable - xConnected) <= 1 && abs(yCable - yConnected) <= 1)
	{
		return true;	//La distance est de 1 ou moins
	}
	return false;	//La distance est superieure strictement a 1
}

//Cette fonction va permettre de savoir si les cables sont ordonnes, c'est a dire que pour chaque cable
//on verifie qu'un cable dans son entourage est deja connecte (sinon la solution n'est pas valide)
bool cableOrdered(const int & xCable, const int & yCable, const std::vector<std::vector<bool>>& cablesVector)
{
	//Avec ces 2 boucles, on va juste verifier si, pour un cable donne, un des voisins (en 8-connexite)
	//est deja relie au backbone (donc est deja connecte)
	for (int i = xCable - 1; i < xCable + 2; i++)
	{
		for (int j = yCable - 1; j < yCable + 2; j++)
		{
			if ((i != xCable || j != yCable) && cablesVector[i][j] == true)	//On a trouve un cable connecte dans le 8 voisinnage
																			//et qui n'est pas sois meme
			{
				return true;
			}
		}
	}
	return false;	//On a pas trouve de cable connecte, la solution n'est pas valide
}

//Cette fonction va permettre de parser la map contenue dans le fichier .in, pour savoir ou se situe
//les murs, les cellules cibles, et les cellules vides (pour ensuite utiliser ces informations).
std::vector<std::vector<int>> parseMap(const std::string & mapName, const int & row, const int & column)
{
	int coordX = 0;
	int coordY = 0;
	std::vector<std::vector<int>> mapMatrix(row, std::vector<int>(column));	//On renverra une matrice d'entiers (cf enum dans arbitre.h)
	std::ifstream file(mapName); //La map a parser (le .in)
	std::string line = "";	//ligne courante

	//Il faut passer les 3 premieres lignes pour ensuite lire la map
	for (int i = 1; i < 4; i++)	//On passe les 3 premiËres lignes car on a deja eu les infos avant (cf infoMap)
	{
		getline(file, line);
	}

	while (getline(file, line))	//Pour toutes les lignes restantes
	{
		coordY = 0;
		for (const char& charElement : line)	//Pour chaque symbole dans la ligne
		{
			if (charElement == '#')	//Selon le symbole, on attribue un entier conformement a l'enum (cf arbitre.h)
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

//Cette fonction va permettre de calculer le score lie au .out donne.
//Cette fonction ne sera appele que si le fichier est valide
int scoreRouter(std::vector<std::vector<int>>& map, const std::vector<std::pair<int, int>>& routerCoord, const int & routerRadius, const int & row, const int & column)
{
	int score = 0;

	for (auto it = routerCoord.begin(); it != routerCoord.end(); it++)	//Pour chaque routeur
	{
		//Double boucles pour parcourir la map
		for (int i = (it->first - routerRadius); i <= (it->first + routerRadius); i++)
		{
			for (int j = (it->second - routerRadius); j <= (it->second + routerRadius); j++)
			{
				//Si la cellule est une cellule cible et qu'il n y a pas de murs entre la cellule et le routeur
				if (map[i][j] == Cell::Wireless && wallPresence(map, it->first, it->second, i, j) == false)
				{
					map[i][j] = Cell::Connected;	//Alors elle est connecte, donc elle rapporte des points
					score += 1000;
				}
			}
		}
	}
	return score;
}

//Cette fonction va permettre de dÈtecter si il y a un mur entre une cellule
//Elle retourne vraie si il y a un mur entre la cellule et le routeur
bool wallPresence(const std::vector<std::vector<int>>& map, const int & xRouter, const int & yRouter, const int & xCell, const int & yCell)
{
	//Selon si le routeur est plus haut ou bas, plus Åa gauche ou Åa droite, alors il faudra aller dans un sens ou un autre.
	//C'est pour cela qu'il y a la condition d'arret et l'incrementation/decrementation avec les conditions ternaires,
	//ce qui permettra d'eviter d'avoir plusieurs tests et de la redondance de code.
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
	//Petit parsage du nom du .out pour en deduire la map correspondante (nom du .out codifie en interne)
	std::string soltmp = solutionFile.substr(16);
	std::string mapFile = "../../inputs/" + soltmp.substr(0, soltmp.find("/")) + ".in";
	std::cout << "Test de la solution : " << solutionFile.substr(16) << std::endl;
	std::cout << "Sur la map : " << mapFile.substr(13) << std::endl << std::endl;

	std::vector<int> data = infoMap(mapFile);	//Contient les informations liees Åa la map
	const int row = data[0];
	const int column = data[1];
	const int radius = data[2];
	const int cableCost = data[3];
	const int routerCost = data[4];
	const int budget = data[5];
	const int xBackbone = data[6];
	const int yBackbone = data[7];

	std::ifstream solution(solutionFile, std::ios::in);
	if (solution)	//On test si le fichier est bien ouvert
	{
		//Parser le fichier de solution
		int nbCable = 0;
		int nbRouters = 0;

		std::vector<std::vector<int>> map = parseMap(mapFile, row, column);
		std::vector<std::vector<bool>> cablesMatrix(row, std::vector<bool>(column));
		std::vector<std::pair<int, int>> routerCoord;

		std::string line = "";
		std::getline(solution, line);	//On prend la ligne, au debut 
		nbCable = stoi(line);

		if (0 <= nbCable && nbCable < row*column)	//On verifie que le nombre de cable est correct
		{
			int a, b;
			for (int i = 0; i < nbCable; i++)	//Pour chaque ligne de coordoonees de cables
			{
				std::getline(solution, line);
				std::istringstream iss(line); //Va nous donner les 2 nombres de la ligne
				if (iss >> a >> b) //a = x et b = y du cable
				{
					if (0 <= a && 0 <= b && a < row && b < column)	//Si les cables sont dans les limites
					{
						if (cableOrdered(a, b, cablesMatrix) || distInf1(a, b, xBackbone, yBackbone))
							//Si les cables sont bien ordonnes, donc qu'il y a un cable deja connecte avant
						{
							cablesMatrix[a][b] = true;	//le cable est ok
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
			nbRouters = stoi(line);	//on le passe en entier

			//On test le budget maintenant car si il est depasse, pas besoin de faire la suite car le fichier est non valide
			if ((nbCable * cableCost + nbRouters * routerCost) > budget)	//Test du budget
			{
				std::cout << "Le budget a ete depasse";
				return 0;
			}

			if (0 <= nbRouters && nbRouters < row*column)	//On test si le nombr de routeurs est bon
			{
				int a, b;
				for (int i = 0; i < nbRouters; i++)	//On fait le parcours des coordoonees de routeurs comme avec les cables ci dessus
				{
					std::getline(solution, line);
					std::istringstream iss(line);
					if (iss >> a >> b)
					{
						if (0 <= a && 0 <= b && a < row && b < column)	//Si le routeur est dans les limites
						{
							if (cablesMatrix[a][b] == true && map[a][b] != Cell::Wall)	//Si le routeur est connecte et pas sur un mur
							{
								routerCoord.push_back(std::pair<int, int>(a, b));	//Alors on l'ajoute au vecteur
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

		//Calcul du score car le fichier est valide
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
	if ((dir = FindFirstFileA((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
	{
		// Invalid directory
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
