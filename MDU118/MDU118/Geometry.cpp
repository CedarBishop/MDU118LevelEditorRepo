#include"Geometry.h"

void Rectangle::setValues(int x, int y)
{
	width = x;
	height = y;
}
void Rectangle::setValues(int x, int y, int z)
{
	width = x;
	height = y;
	breadth = z;
}
int Rectangle::area()
{
	return width * height;
}
int Rectangle::volume()
{
	return width * height * breadth;
}
