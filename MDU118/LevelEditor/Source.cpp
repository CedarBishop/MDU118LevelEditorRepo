#include"app.h"
#include<iostream>

using namespace std;
int main()
{
	App Game("Level Editor", 1920, 1080, 32);
	if (!Game.Init())
	{
		printf("Game failed");
		return 1;
	}
	else 
	{
		Game.Run();
	}
	return 0;
}