#include "Header.h"
#include "Matrix.h"
#include "IO.h"
#include "Algo.h"

int main()
{
	//Chemin
	std::string path = "../../inputs/";
	//Nom du fichier
	std::string filename = "simple_example";

	std::string fichierInput = path+filename+".in"; // fichier d'input
	std::string fichierOutput = path + filename + ".out"; // fichier d'output
	IO io; // objet pour les I/O

	int * datas = io.initializeData(fichierInput); // datas = tableau de la forme [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]

	Matrix mapOriginale(datas[0], datas[1]); // matrice originale nbLignes x nbColonnes
	Matrix map(datas[0], datas[1]); // matrice nbLignes x nbColonnes

	int * xyBackbone = new int[2]{ datas[6], datas[7] };

	Algo algo("theoreme_de_soyer", map, datas[2], datas[3], datas[4], datas[5], datas[5], xyBackbone); // type, rayonRouteurs, prixCable, prixRouteur, budgetMax, budgetOriginal, [xBackbone, yBackbone]
	
	// remplissage de la matrice
	io.initializeMap(map, fichierInput);

	// impression de la matrice
	//std::cout << map << std::endl;

	algo.run(); // lancemet de l'algo

	std::cout << map << std::endl;
	//io.generateOutput(map, fichierOutput);
									

	// ###################### CREATION OUTPUT ######################
	Matrix mapRouteursTest(datas[0], datas[1]); // matrice nbLignes x nbColonnes
	// remplissage de la matrice
	io.initializeMap(mapRouteursTest, fichierInput);
	// placement de l'emetteur
	mapRouteursTest(2, 7) = -2;
	// placement des 2 routeurs
	mapRouteursTest(3, 6) = 3;
	mapRouteursTest(3, 9) = 3;
	// placement des cables. /!\ il y a un cable sous les routeurs aussi ! Du coup il ya un cable à (3, 6) et (3, 9)
	mapRouteursTest(3, 8) = 4;
	std::string fichierOutputTest = path + "test.out"; // fichier d'output
	io.generateOutput(mapRouteursTest, fichierOutputTest);
	

	return 0;
}
