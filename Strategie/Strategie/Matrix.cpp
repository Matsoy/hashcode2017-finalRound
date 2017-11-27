#include "Header.h"
#include "Matrix.h"

Matrix::Matrix(int rows, int cols) : mRows(rows), mCols(cols), mData(rows * cols)
{
}

int& Matrix::operator()(int i, int j)
{
	if ((i * mCols + j) >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes");
	return mData[i * mCols + j];
}

int Matrix::operator()(int i, int j) const
{
	if ((i * mCols + j) >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes");
	return mData[i * mCols + j];
}

int& Matrix::operator()(int k)
{
	if (k >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes");
	return mData[k];
}

int Matrix::operator()(int k) const
{
	if (k >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes");
	return mData[k];
}

// Accesseurs
int Matrix::getRows()
{
	return mRows;
}

int Matrix::getCols()
{
	return mCols;
}

/*
* test si la matrice est vide
*
* @return true si la matrice remplie de 0
*			false sinon
*/
bool Matrix::isEmpty()
{
	for (int i = 0; i < mData.size(); i++)
	{
		if (mData[i] != 0) return false;
	}

	return true;
}

/*
* calcule la somme des valeurs de la matrice
*
* @return la somme des valeurs de la matrice
*/
int Matrix::sum()
{
	int sum = 0;

	for (int i = 0; i < mData.size(); i++)
	{
		sum += mData[i];
	}

	return sum;
}

/*
* renvoie la coordonnees matrix[x][y] correspondant au vector[z]
*
* @param z index dans le vecteur
* @return matrix[x][x] <==> vector[z]
*/
int * Matrix::xy(int z)
{
	int * xy = new int[2];

	int x = z / mCols;  // indice ligne
	int y = z - x * mCols; // indice colonne

	xy[0] = x;
	xy[1] = y;

	return xy;
}

//Pour afficher une matrice
std::ostream & operator<<(std::ostream & os, const Matrix &mat)
{
	int m = 0, n = 0, cpt = 0;

	for (int j = 0; j <( mat.mRows * mat.mCols); j++)
	{
		m = cpt / mat.mCols;
		n = cpt % mat.mCols;

		if(n == 0) os << "[ "; // si 1er elem

		if ((n == mat.mCols - 1)) // si dernier elem
		{
			os << mat(m, n) << " ]" << std::endl;
		}
		else os << mat(m, n) << ", ";
		cpt++;
	}

	return os;
}

