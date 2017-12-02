#include "Header.h"
#include "Matrix.h"
#include "IO.h"
#include "Algo.h"

int main()
{
	try {
		//Nom de la carte
		std::string mapName = "lets_go_higher";
		std::string method = "random";


		std::cout << "  Carte =\t\t\t" << mapName << std::endl;

		//Chemins
		std::string pathInput = "../../inputs/";
		std::string pathOutput = "../../solutions/" + mapName + "/";

		std::string inputFile = pathInput + mapName + ".in"; // fichier d'input

		IO io; // objet pour les I/O

		if (io.isFileExist(inputFile))
		{
			int * datas = io.initializeData(inputFile); // datas = tableau de la forme [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]

			Matrix mapOriginale(datas[0], datas[1]); // matrice originale nbLignes x nbColonnes
			Matrix map(datas[0], datas[1]); // matrice qu'on va manipuler nbLignes x nbColonnes

			int * xyBackbone = new int[2]{ datas[6], datas[7] };

			Algo algo(method, mapName, map, datas[2], datas[3], datas[4], datas[5], datas[5], xyBackbone); // type, rayonRouteurs, prixCable, prixRouteur, budgetMax, budgetOriginal, [xBackbone, yBackbone]

																									// remplissage de la matrice
			io.initializeMap(map, inputFile);

			algo.run(); // lancement de l'algo

						// ###################### CREATION OUTPUT ######################
			std::cout << std::endl;
			std::string pathBeginning = pathOutput + algo.getMethod() + "-" + mapName;
			io.generateOutput(map, pathBeginning);
		}
		else std::cout << "ERREUR: Le fichier " << inputFile << " n'existe pas !" << std::endl;

	}
	catch (const std::exception& ex) {
		std::cout << "EXCEPTION => " << ex.what() << std::endl;
	}
	catch (const std::string& ex) {
		std::cout << "EXCEPTION => " << ex << std::endl;
	}

	return 0;
}
