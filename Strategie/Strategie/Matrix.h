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
	int getRows();
	int getCols();

	bool isEmpty();
	int sum();
	int * xy(int z);

	friend std::ostream & operator<<(std::ostream &os, const Matrix &mat);
};