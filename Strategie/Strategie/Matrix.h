#pragma once
#include "Header.h"

/*
* Classe qui imite l'utilisation d'une matrice depuis un vecteur d'entiers
*/
class Matrix
{
private:
	int mRows; // nombre de lignes de la matrice
	int mCols; // nombre de colonnes de la matrice
	std::vector<int> mData; // vecteurs

public:
	/*
	* Constructeur
	*/
	Matrix(int rows, int cols);
	int& operator()(int i, int j);
	int operator()(int i, int j) const;
	int& operator()(int k);
	int operator()(int k) const;

	/*
	* Accesseurs
	*/
	int getRows() const;
	int getCols() const;

	/*
	* Mutateurs
	*/
	void setRows(int r);
	void setCols(int c);

	/*
	* test si la matrice est vide
	*
	* @return true si la matrice remplie de 0
	*			false sinon
	*/
	bool isEmpty() const;
	/*
	* calcule la somme des valeurs de la matrice
	*
	* @return la somme des valeurs de la matrice
	*/
	int sum() const;
	/*
	* renvoie la coordonnees matrix[x][y] correspondant au vector[z]
	*
	* @param z index dans le vecteur
	* @return matrix[x][x] <==> vector[z]
	*/
	int * xy(int z);
	/*
	* retourne les entrees dans chaque colonne dans le sens haut / bas.
	* Les lignes sont conservees, mais apparaissent dans un ordre different de celui d'avant.
	*/
	void reverse();
	/*
	* Pour afficher une matrice
	*
	* @param os l'objet ostream
	* @param mat l'objet matrice
	* @return une reference sur un objet ostream contenant les informations de la matrice mat
	*/
	friend std::ostream & operator<<(std::ostream &os, const Matrix &mat);
};

// Accesseurs inline

inline
int Matrix::getRows() const
{
	return mRows;
}

inline
int Matrix::getCols() const
{
	return mCols;
}

// #####

inline
int& Matrix::operator()(int i, int j)
{
	if ((i * mCols + j) >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes pour [i,j] = [" + std::to_string(i) + "," + std::to_string(j) + "]");
	return mData[i * mCols + j];
}

inline
int Matrix::operator()(int i, int j) const
{
	if ((i * mCols + j) >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes pour [i,j] = [" + std::to_string(i) + "," + std::to_string(j) + "]");
	return mData[i * mCols + j];
}

inline
int& Matrix::operator()(int k)
{
	if (k >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes pour [k] = [" + std::to_string(k) + "]");
	return mData[k];
}

inline
int Matrix::operator()(int k) const
{
	if (k >= (mRows * mCols)) throw std::out_of_range("Depassement des bornes pour [k] = [" + std::to_string(k) + "]");
	return mData[k];
}