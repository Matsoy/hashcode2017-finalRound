#include "Matrix.h"

Matrix::Matrix(int rows, int cols) : mRows(rows), mCols(cols), mData(rows * cols)
{
}

int& Matrix::operator()(int i, int j)
{
	return mData[i * mCols + j];
}

int Matrix::operator()(int i, int j) const
{
	return mData[i * mCols + j];
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

//Pour afficher une matrice
std::ostream & operator<<(std::ostream & os, const Matrix &mat)
{
	int m = 0, n = 0, cpt = 0;

	for (unsigned int j = 0; j < mat.mRows*mat.mCols; j++)
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