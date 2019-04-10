#include <iostream>
#include <time.h>
using namespace std;

int main()
{
	srand(time(NULL));
	
	int i;

	i = rand();

	cout << "i = " << i << endl;

	cout << "address of i = " << &i << endl;

	int* p;

	p = &i;

	cout << "p = " << p << endl;

	cout << "*p = " << *p << endl;

	cout << "\n*****************Pointers and arrays ******************\n";

	const int ROW = 12;
	const int COL = 7;


	cout << "\n*****************Static Arrays *********************\n";

	int myInts[COL];

	for(int i : myInts)
	{
		i = rand();
		cout << i << endl;
	}

	cout << "\n*****************1D dynamic arrays *********************\n";

	int* q;

	q = new int[COL];

	for (int i = 0; i < COL; i++)
	{
		q[i] = rand();
		cout << "q[" << i << "] = " << q[i] << endl;
	}

	delete q;

	cout << "\n*****************2D dynamic arrays *********************\n";


	int** r;
	
	/*int* rowPtr = new int[ROW];
	r = &rowPtr;*/

	r = new int*[ROW];

	for (int i = 0; i < ROW; i++)
	{
		r[i] = new int[COL];
	}

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			r[i][j] = rand();
			cout << r[i][j] << "\t";
		}
		cout << "\n";
	}
	for (int i = 0; i < ROW; i++)
	{
		delete[] r[i];
	}

	delete[] r;

	cin.get();
	return 0;
}
