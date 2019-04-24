#include <iostream>

using namespace std;

const double PI = 3.141;

class Rectangle 
{
	int width, height;
public:
	Rectangle();

	Rectangle(int);

	Rectangle(int,int);

	int area()
	{
		return (width * height);
	}
};
class Circle
{
	double radius;
public:
	Circle(double r) : radius(r) {}
	double area() 
	{
		return radius * radius * PI;
	}
};

class Cylinder
{
	Circle base;
	double height;
public:
	Cylinder(double r, double h) : base(r), height(h) {}
	double volume() { return base.area() * height; }

};

int main()
{
	Rectangle rectA;
	Rectangle rectB(3,4);

	cout << "RectA area is " << rectA.area() << endl;
	cout << "rectB area is " << rectB.area() << endl;

	Rectangle square(1);
	Rectangle square2 = 2;
	Rectangle square3{ 3, 2 };
	Rectangle square4 = { 4, 2 };

	Circle bar(5);
	cout << "bars area " << bar.area() << endl;


	Cylinder foo(10, 20);
	cout << "foos area " << foo.volume() << endl;

	Rectangle rectC(4,5);
	Rectangle *p, *q, *r;

	p = &rectC;
	cout << "" << rectC.area() << endl;
	cout << "" << p->area() << endl;

	q = new Rectangle(5,6);
	cout << "q's area: " << (*q).area() << endl;

	r = new Rectangle[2]{ {1,2}, {3,4} };
	cout << "r[0] area" << r[0].area() << endl;
	cout << "r[1] area " << (r + 1)->area() << endl;
	
	delete q;
	delete r;
	
	cin.get();
	return 0;
}

Rectangle::Rectangle()
{
	cout << "parameter constructor invoked" << endl;
	width = 5;
	height = 5;
}

Rectangle::Rectangle(int a)
{
	cout << "one parameter constructor invoked \n";
	width = height = a;	
}

Rectangle::Rectangle(int a, int b)
{
	cout << "two parameter contructor invoked\n";
	width = a;
	height = b;
}

