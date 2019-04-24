#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
	bool i, j;
	ifstream myfile("example.txt");
	if (myfile.is_open())
	{
		/*while ((myfile, line))
		{
			cout << line << '\n';
		}
		myfile.close();*/
		myfile >> i >> j;
		cout << i << "\t" << j;
	}

	else cout << "Unable to open file";
	cin.get();
	return 0;
}