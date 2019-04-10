#include <iostream>
using namespace std;

int sum(int a, int b)
{
	return( a + b);
}

double sum(double a, double b)
{
	return(a + b);
}

template <class T>
T add(T x, T y)
{
	return (x + y);
}

int main()
{
	cout << sum(10,20) << "\n";
	cout << sum(15.2, 20.4) << "\n";

	cout << add<int>(1,2) << endl;
	cout << add<double>(5.5,7.6) << endl;

	cout << add(1, 2) << endl;
	cout << add(5.5, 7.6) << endl;
	cout << "Hello " << "World" << endl;
	return 0;
}