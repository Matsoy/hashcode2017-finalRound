#include "Algo.h"



Algo::Algo(std::string type, Matrix & map, int rayonRouteurs, int prixCable, int prixRouteur, int budget, int budgetOriginal, int * xyBackbone) : aType(type), aMap(map), aRayonRouteurs(rayonRouteurs),  aPrixCable(prixCable),  aPrixRouteur(prixRouteur), aBudget(budget),  aBudgetOriginal(budgetOriginal), aBackbone(xyBackbone)
{
}

// Accesseurs
std::string Algo::getType()
{
	return aType;
}

Matrix & Algo::getMap()
{
	return aMap;
}

int Algo::getRayonRouteurs()
{
	return aRayonRouteurs;
}

int Algo::getPrixCable()
{
	return aPrixCable;
}

int Algo::getPrixRouteur()
{
	return aPrixRouteur;
}
int Algo::getBudget()
{
	return aBudget;
}

int Algo::getBudgetOriginal()
{
	return aBudgetOriginal;
}
int * Algo::getBackbone()
{
	//std::cout << "dans algo" << aBackbone[1] << std::endl;
	return aBackbone;
}

void kruskal(Matrix & m, int * backbone)
{
	int new_id = 0;
	// calcul des nouvelles distances de routeur

}

void Algo::run()
{
	if (aType == "theoreme_de_soyer")
	{
		std::cout << "aType = " << aType << std::endl;

		int max_num_routers = aBudget / aPrixRouteur; // nb max de routeurs possibles
		std::cout << "max_num_routers = " << max_num_routers << std::endl;
		int _budget = aBudget;
		int R = aRayonRouteurs;

		//int cost, succ, routers, idx, idy, dists = kruskal(aMap, aBackbone);











	}
}
