#include "Element.h"

// un element (classe mère)

Element::Element(int x, int y, int value): x(), y(), value()
{
}

void Element::setX(int newX)
{
	x = newX;
}

void Element::setY(int newY)
{
	y = newY;
}

void Element::setXY(int newX, int newY)
{
	x = newX;
	y = newY;
}

int Element::getValue()
{
	return value;
}
