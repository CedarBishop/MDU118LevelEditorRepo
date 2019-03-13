#include<iostream>
#include"Geometry.h"
using namespace std;

int main()
{
	int width;
	int height;
	int breadth;
	cin >> width;
	cin >> height;
	cin >> breadth;
	Rectangle obj;
	obj.setValues(width, height);
	cout << "area = " << obj.area() << endl;
	obj.setValues(width, height, breadth);
	cout << "volume = " << obj.volume() << endl;
	cin.get();
	return 0;
}
