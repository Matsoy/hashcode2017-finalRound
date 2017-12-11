#include "Header.h"
#include "Matrix.h"
#include "IO.h"
#include "Algo.h"

int main()
{
	int nbSimulations = 1;

	// affichage des meilleurs scores

	int bestScore1 = 21962790;
	int bestScore2 = 58307050;
	int bestScore3 = 174527047;
	int bestScore4 = 290174532;
	int scoreTotal = bestScore1 + bestScore2 + bestScore3 + bestScore4;
	std::cout << "  Meilleurs scores";
	std::cout << "\tcharleston_road\t\t" << bestScore1 << std::endl;
	std::cout << "\t\t\true_de_londres\t\t" << bestScore2 << std::endl;
	std::cout << "\t\t\topera\t\t\t" << bestScore3 << std::endl;
	std::cout << "\t\t\tlets_go_higher\t\t" << bestScore4 << std::endl;
	std::cout << "\t\t\t---------------------------------" << std::endl;
	std::cout << "\t\t\ttotal\t\t\t" << scoreTotal << "\n" << std::endl;

	//Nom de la carte
	std::string mapName = "lets_go_higher";
	std::string method = "gaussian_blur";


	for (int i = 1; i <= nbSimulations; i++)
	{
		std::cout << i << std::endl;

		clock_t tStart = clock();

		try {
			std::cout << "  Carte\t\t\t" << mapName << std::endl;

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
				Matrix mapSolution(datas[0], datas[1]); // matrice solution nbLignes x nbColonnes

				int * xyBackbone = new int[2]{ datas[6], datas[7] };

				Algo algo(method, mapName, map, mapSolution, datas[2], datas[3], datas[4], datas[5], datas[5], xyBackbone); // type, rayonRouteurs, prixCable, prixRouteur, budgetMax, budgetOriginal, [xBackbone, yBackbone]

				// remplissage de la matrice
				io.initializeMap(map, inputFile);



				
				// ###################### CABLAGE SUR SOLUTION ######################
				std::vector<int *> routeurs;
				io.initializeMap(mapSolution, inputFile);
				std::string solutionFile = "../../solutions/lets_go_higher/gaussian_blur-1512722223-290177232.out";
				int exNbCables = io.initializeMapFromSolution(mapSolution, solutionFile, routeurs);
				algo.setMapSolution(mapSolution);
				algo.centroid(routeurs, io, pathOutput + algo.getMethod());

				// ##################################################################
				

				
				/*
				// lancement de l'algo en lui fournissant le meilleur score de la carte correspondante
				if (mapName == "charleston_road") algo.run(bestScore1);
				else if (mapName == "rue_de_londres") algo.run(bestScore2);
				else if (mapName == "opera") algo.run(bestScore3);
				else if (mapName == "lets_go_higher") algo.run(bestScore4);
				else if (mapName == "simple_example") algo.run(10000000);
				*/

				// impression du temps ecoule
				printf("  Temps d'execution:\t %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

				

				// ###################### CREATION OUTPUT ######################
				std::cout << std::endl;
				std::string pathBeginning = pathOutput + algo.getMethod();
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

	}

	return 0;
}
