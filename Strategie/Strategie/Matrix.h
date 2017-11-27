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

	// accesseurs
	int getRows();
	int getCols();

	bool isEmpty();
	int Matrix::sum();

	friend std::ostream & operator<<(std::ostream &os, const Matrix &mat);
};