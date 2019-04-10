#include <iostream>
#include <new>
using namespace std;

int main()
{
	int i, n;
	int* p;
	cout << "How many numbers would you like to type?";
	cin >> i;
	p = new (nothrow) int[i];
	if (p == nullptr)
	{
		cout << "Error, memory could not be allocated";
	}
	else
	{
		for (int n = 0; n < i; n++)
		{
			cout << "Enter Number: ";
			cin >> p[n];
		}
		cout << "You have entered: ";
		for (int n = 0; n < i; n++)
		{
			cout << p[n] << ", ";
		}
		delete[] p;
	}


	cin.get();
	return 0;
}