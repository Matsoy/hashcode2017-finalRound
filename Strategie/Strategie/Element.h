#pragma once

class Element
{
public:
	Element(int x = 0, int y = 0);
	void setX(int newX); 
	void setY(int newY);
	void setXY(int newX, int newY);

private:
	int x;
	int y;
};