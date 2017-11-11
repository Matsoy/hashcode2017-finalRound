#pragma once
#include "Header.h"
#include "Element.h"

template<int R, int C> class Matrix
{
protected:
	Element tab[R][C] = {};

public:
	Matrix();
	Matrix(std::initializer_list<Element> liste);
	virtual ~Matrix();

	Element& operator()(const int &i, const int &j);
	Element operator()(const int &i, const int &j) const;

	template<int R, int C> friend std::ostream & operator<<(std::ostream &os, const Matrix<R, C> &mat);
};

template<int R, int C> std::ostream & operator<<(std::ostream &os, const Matrix<R, C> &mat);

template<int R, int C> Matrix<R, C>::Matrix()
{
}

//Constructeur 
template<int R, int C> Matrix<R, C>::Matrix(std::initializer_list<Element> liste)
{
	if (liste.size() > R*C)
	{
		throw std::exception("Exception: Trop d'objets Element dans la liste");
	}

	int i = 0, j = 0, cpt = 0;

	for (const Element &elem : liste)
	{
		i = cpt / C;	//Formules qui evite la double boucle de remplissage
		j = cpt % C;
		tab[i][j] = elem;
		cpt++;
	}
}

//Destructeur
template<int R, int C> Matrix<R, C>::~Matrix()
{
}

//Les foncteurs pour acceder aux elements de la matrice
template<int R, int C> Element & Matrix<R, C>::operator()(const int &i, const int &j)
{
	if (i >= R || j >= C)
	{
		throw std::exception("Exception: indice en dehors des bornes");
	}
	return tab[i][j];
}

template<int R, int C> Element Matrix<R, C>::operator()(const int &i, const int &j) const
{
	if (i >= R || j >= C)
	{
		throw std::exception("Exception: indice en dehors des bornes");
	}
	return tab[i][j];
}

//Pour afficher une matrice
template<int R, int C> std::ostream & operator<<(std::ostream & os, const Matrix<R, C> & mat)
{
	for (int i = 0; i < R; i++)
	{
		os << "[ ";
		for (int j = 0; j < C; j++)
		{
			if (j == (C - 1))
			{
				os << mat.tab[i][j];	//On met pas de ", " apres le dernier element
			}
			else
			{
				os << mat.tab[i][j] << ", ";
			}
		}
		os << "]" << std::endl;
	}
	return os;
}