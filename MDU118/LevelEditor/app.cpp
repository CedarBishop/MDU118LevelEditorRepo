#include "app.h"
#include <iostream>

// Constructor, we are using it to create the window and viewport
App::App(char const* Title, int screenWidth, int screenHeight, int screenBPP)
{
	window.create(sf::VideoMode(screenWidth,screenHeight,screenBPP),Title);
	window.setFramerateLimit(0);
	view = window.getDefaultView();
}

App::~App()
{

}


// Initialise memeber variables here
bool App::Init()
{	
	
	InitializeCircles();	
	InitializePaddle();
	backgroundTexture.loadFromFile("Images/Brick.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(7, 6);
	srand(time(NULL));
	sign1 = 2 * (rand() % 2) - 1;
	sign2 = 2 * (rand() % 2) - 1;
	speed = Vector2f(sign1 * (rand() % 201 + 200), sign2 * (rand() % 201 + 200));	

	paddleVelocity = 1000.0f;
	
	return true;
}



// Add game logic here
void App::Update()
{
	deltaTime = clock.restart().asSeconds();
	
	CircleMovement();

	PaddleMovement();
	
}

void App::Draw()
{

	window.clear();
	window.setView(view);

	// Sprite Drawing Starts here 

	window.draw(backgroundSprite);
	window.draw(ballSprite);
	for (int i = 0; i < N; i++)
	{
		window.draw(circle[i]);
	}
	window.draw(paddle);
	
	
	// Ends here
	window.display();
}

// Input
void App::HandleEvents()
{
	if (event.type == sf::Event::Closed)
	{
		window.close();
	}

	Vector2i localMousePosition = Mouse::getPosition(window);
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		if (paddle.getGlobalBounds().contains(Vector2f(localMousePosition)))
		{
			paddle.setFillColor(Color::Red);
		}
	}
}

void App::Run()
{
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			HandleEvents();
		}
		Update();
		Draw();
	}
}

void App::InitializeCircles()
{
	radius = window.getSize().x * 0.02f;
	ballTexture.loadFromFile("Images/TennisBall.png");
	
	for (int i = 0; i < N; i++)
	{
		circle[i].setRadius(radius);
		circle[i].setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));
		circle[i].setOrigin(Vector2f(radius, radius));
		circle[i].setTexture(&ballTexture);
	}	

	xSpeed[0] = 500;
	ySpeed[0] = 500;
	xSpeed[1] = -500;
	ySpeed[1] = -500;

	//ballSprite.setTexture(ballTexture);
}

void App::InitializePaddle()
{
	paddleSize = Vector2f(0.1f * window.getSize().x, 0.01f * window.getSize().y);
	paddle.setSize(paddleSize);
	paddle.setOrigin(Vector2f(paddle.getSize().x / 2, paddle.getSize().x / 2));
	paddle.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y * 0.85f));
	paddleTexture.loadFromFile("Images/PaddleTexture.jpg");
	paddle.setTexture(&paddleTexture);

}

void App::CircleMovement()
{
	for (int i = 0; i < N; i++)
	{
		circle[i].move(xSpeed[i] * deltaTime, ySpeed[i] * deltaTime);

		// Border Collision Right
		if (circle[i].getPosition().x > (window.getSize().x - radius))
		{
			circle[i].setPosition(window.getSize().x - radius, circle[i].getPosition().y);
			xSpeed[i] *= -1;
		}

		// Border Collision Left
		if (circle[i].getPosition().x < radius)
		{
			circle[i].setPosition(radius, circle[i].getPosition().y);
			xSpeed[i] *= -1;
		}

		//Border Collision Bottom
		if (circle[i].getPosition().y > (window.getSize().y - radius))
		{
			circle[i].setPosition(circle[i].getPosition().x, window.getSize().y - radius);
			ySpeed[i] *= -1;
		}

		// Border Collision Top
		if (circle[i].getPosition().y < radius)
		{
			circle[i].setPosition(circle[i].getPosition().x, radius);
			ySpeed[i] *= -1;
		}

		// Collision with paddle

		if (circle[i].getGlobalBounds().intersects(paddle.getGlobalBounds()))
		{
			if ((circle[i].getPosition().x + radius > paddle.getSize().x - (paddleSize.x / 2)) && (circle[i].getPosition().x + radius < paddle.getSize().x - (paddleSize.x / 1.9f)) || (circle[i].getPosition().x - radius < paddle.getSize().x + (paddleSize.x / 2)) && (circle[i].getPosition().x - radius > paddle.getSize().x + (paddleSize.x / 1.9f)))
			{
				xSpeed[i] *= -1;
			}
			else
			{
				ySpeed[i] *= -1;
			}
		}
		

		/*if ((circle[i].getPosition().y + radius > paddle.getPosition().y - paddleSize.y 
			&& circle[i].getPosition().y + radius < paddle.getPosition().y + paddleSize.y)
			&& (circle[i].getPosition().x > paddle.getPosition().x - paddleSize.x
				&& circle[i].getPosition().x < paddle.getPosition().x + paddleSize.x))
		{
			ySpeed[i] *= -1;
		}*/
		
	}
}

void App::PaddleMovement()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		paddle.move(-paddleVelocity * deltaTime, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		paddle.move(paddleVelocity * deltaTime, 0);
	}

	//Player Border Collision
	if (paddle.getPosition().x > (window.getSize().x - paddleSize.x / 2)) //Right
	{
		paddle.setPosition(window.getSize().x - paddleSize.x / 2, paddle.getPosition().y);
	}
	if (paddle.getPosition().x < paddleSize.x / 2) // Left
	{
		paddle.setPosition(paddleSize.x / 2, paddle.getPosition().y);
	}
}


