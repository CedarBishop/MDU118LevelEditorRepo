#include <iostream>
using namespace std;

int subtraction(int x, int y)
{
	return (x - y);
}

int addition(int x, int y)
{
	return (x + y);
}

int operation(int x, int y, int (*funcToCall)(int, int))
{
	int g;
	g = (*funcToCall)(x, y);
	return g;
}

int main()
{
	int firstValue, secondValue;
	int* myPointer;
	myPointer = &firstValue;
	*myPointer = 10;
	cout << "myPointer is " << myPointer << endl;
	cout << "*myPointer = " << *myPointer << endl;
	cout << "firstValue is " << firstValue << endl;

	myPointer = &secondValue;
	*myPointer = 20;
	cout << "myPointer is " << myPointer << endl;
	cout << "*myPointer = " << *myPointer << endl;
	cout << "secondValue is " << secondValue << endl;
	//cin >> secondValue;

	int numbers[5];
	int* p;
	p = numbers;
	*p = 10; // same as number[0] = 10;
	p++;

	*p = 20;
	p = &numbers[2];
	*p = 30;

	p = numbers + 3;
	*p = 40;

	p = numbers;
	*(p + 4) = 50;

	for (int i = 0; i < 5; i++)
	{
		cout << numbers[i] << ", " << endl;
	}

	for (int i = 0; i < 5; i++)
	{
		cout << *(p + i) << ", " << endl;
	}
/*
	cout << numbers << endl;
	cout << &numbers[0] << endl;*/
	
	int m, n;
	int(*minus)(int, int);
	minus = subtraction;

	m = operation(7,5,addition);
	n = operation(20, m, minus);
	
	
	cin.get();
	return 0;
}