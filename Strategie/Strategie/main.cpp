#include "Header.h"
#include "Matrix.h"
#include "IO.h"
#include "Algo.h"

int main()
{
	std::string fichierInput = "../../inputs/extra_simple_example.in"; // fichier d'input
	IO io; // objet pour les I/O

	int * datas = io.initializeData(fichierInput); // datas = tableau de la forme [nbLignes, nbColonnes, rayonRouteurs, prixCable, prixRouteur, budgetMax, xBackbone, yBackbone]

	Matrix mapOriginale(datas[0], datas[1]); // matrice originale nbLignes x nbColonnes
	Matrix map(datas[0], datas[1]); // matrice nbLignes x nbColonnes

	int * xyBackbone = new int[2]{ datas[6], datas[7] };

	Algo algo("theoreme_de_soyer", map, datas[2], datas[3], datas[4], datas[5], datas[5], xyBackbone); // type, rayonRouteurs, prixCable, prixRouteur, budgetMax, budgetOriginal, [xBackbone, yBackbone]
	
	// remplissage de la matrice
	io.initializeMap(map, fichierInput);

	// impression de la matrice
	std::cout << map << std::endl;

	algo.run(); // lancemet de l'algo
									
}
