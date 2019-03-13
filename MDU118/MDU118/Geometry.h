#pragma once
class Rectangle
{
private:
	int width;
	int height;
	int breadth;

public:
	int area();
	int volume();
	void setValues(int x, int y);
	void setValues(int, int, int);
};
