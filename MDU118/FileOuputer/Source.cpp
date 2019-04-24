// writing on a text file
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	bool x = true;
	bool y = false;

	ofstream myfile("C:/Users/cbishop5/Documents/GitHub/MDU118LevelEditorRepo/MDU118/FileReader/example.txt");
	if (myfile.is_open())
	{
		myfile << x << "\t" << y;
		/*x = 3;
		y = 4;
		myfile << x << "\t" << y;*/
		myfile.close();
	}
	else cout << "Unable to open file";
	return 0;
}