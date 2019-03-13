#include <SFML/Graphics.hpp>	
using namespace sf;
int main()
{
	RenderWindow window(VideoMode(1920, 1080), "Hello World");
	CircleShape circle(200);
	//main game loop
	while (window.isOpen())
	{

		// user input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}


		//game logic




		//rendering
		window.clear();
		window.draw(circle);
		window.display();
		
	}
	return 0;
}

