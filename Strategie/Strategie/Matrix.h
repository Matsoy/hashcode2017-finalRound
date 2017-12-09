#pragma once
#include "Header.h"

class Matrix
{
private:
	int mRows;
	int mCols;
	std::vector<int> mData;

public:
	Matrix(int rows, int cols);
	int& operator()(int i, int j);
	int operator()(int i, int j) const;
	int& operator()(int k);
	int operator()(int k) const;

	// accesseurs
	int getRows() const;
	int getCols() const;

	// mutateurs
	int setRows(int r);
	int setCols(int c);

	bool isEmpty() const;
	int sum() const;
	int * xy(int z);
	void reverse();

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