#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
int main()
{
	RenderWindow window(VideoMode(1200,800), "Hello World");
	Image image;
	Texture texture;
	if (!image.loadFromFile("Images/GreyX.png"))
	{
		std::cout << "Error" << std::endl;
	}

	texture.loadFromImage(image);
	Sprite sprite;
	sprite.setTexture(texture);
	Vector2f scale((float)window.getSize().x / image.getSize().x, (float)window.getSize().y / image.getSize().y);
	sprite.setScale(scale);
	

	Color colour[5][5];
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			colour[i][j] = image.getPixel((image.getSize().x / 5) * j + 1 , ((image.getSize().y / 5) * i + 1));
			if (colour[i][j].a > 100)
			{
				std::cout << "Draw Brick" << std::endl;
			}
			else
			{
				std::cout << "Point is transparent\n";
			}
		}
	}

	


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
		//window.draw(sprite);
		window.display();
		
	}
	return 0;
}

