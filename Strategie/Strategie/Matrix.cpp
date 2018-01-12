#include "Header.h"
#include "Matrix.h"

/*
* Constructeurs
*/
Matrix::Matrix(int rows, int cols) : mRows(rows), mCols(cols), mData(rows * cols)
{
}

/*
* Mutateurs
*/
void Matrix::setRows(int r)
{
	mRows = r;
}

void Matrix::setCols(int c)
{
	mCols = c;
}

/*
* test si la matrice est vide
*
* @return true si la matrice remplie de 0
*			false sinon
*/
bool Matrix::isEmpty() const
{
	int size = mData.size();
	for (int i = 0; i < size; i++)
	{
		if (mData[i] != 0) {
			return false;
		}
	}
	return true;
}

/*
* calcule la somme des valeurs de la matrice
*
* @return la somme des valeurs de la matrice
*/
int Matrix::sum() const
{
	int sum = 0;
	int size = mData.size();
	for (int i = 0; i < size; i++)
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

/*
* retourne les entrees dans chaque colonne dans le sens haut / bas.
* Les lignes sont conservees, mais apparaissent dans un ordre different de celui d'avant.
*/
void Matrix::reverse()
{
	for (int c = 0; c < mCols; c++) // pour chaque colonne
	{
		int condition = (mRows / 2);
		for (int r = 0; r < condition; r++) // on parcourt seulement la moitie des lignes
		{
			// on inverse les lignes
			int tmp = mData[r * mCols + c];

			mData[r * mCols + c] = mData[(mRows - r - 1) * mCols + c];
			mData[(mRows - r - 1) * mCols + c] = tmp;
		}
	}
}

/*
* Pour afficher une matrice
*
* @param os l'objet ostream
* @param mat l'objet matrice
* @return une reference sur un objet ostream contenant les informations de la matrice mat
*/
std::ostream & operator<<(std::ostream & os, const Matrix &mat)
{
	int m = 0, n = 0, cpt = 0;
	int condition = mat.mRows * mat.mCols;
	for (int j = 0; j < condition; j++)
	{
		m = cpt / mat.mCols;
		n = cpt % mat.mCols;

		// si 1er elem
		if (n == 0) {
			os << "[ ";
		}

		// si dernier elem
		if ((n == mat.mCols - 1)) {
			os << mat(m, n) << " ]" << std::endl;
		}
		else {
			os << mat(m, n) << ", ";
		}

		cpt++;
	}

	return os;
}

