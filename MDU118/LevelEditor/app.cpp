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
	srand(time(NULL));
	InitializeCircles();	
	InitializePaddle();
	InitializeBricks();
	backgroundTexture.loadFromFile("Images/Brick.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(7, 6);	
	
	return true;
}



// Add game logic here
void App::Update()
{
	deltaTime = clock.restart().asSeconds();
	
	CircleMovement();
	PaddleMovement();
	PaddleCollision();	
}

void App::Draw()
{
	window.clear();
	window.setView(view);

	// Sprite Drawing Starts here 

	window.draw(backgroundSprite);
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			if (collided[i][j] == false)
			{
				window.draw(bricks[i][j]);
			}			
		}
	}
	window.draw(ball);
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

	ball.setRadius(radius);
	ball.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	ball.setOrigin(Vector2f(radius, radius));
	ball.setTexture(&ballTexture);		

	sign = 2 * (rand() % 2) - 1;
	sign2 = 2 * (rand() % 2) - 1;
	speed = Vector2f(sign * (rand() % 201 + 300), sign2 * (rand() % 201 + 300));
	
	//ballSprite.setTexture(ballTexture);
}

void App::InitializePaddle()
{
	paddleSize = Vector2f(0.1f * window.getSize().x, 0.05f * window.getSize().y);
	paddle.setSize(paddleSize);
	paddle.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y * 0.85f));
	paddleTexture.loadFromFile("Images/PaddleTexture.jpg");
	paddle.setTexture(&paddleTexture);
	paddleVelocity = 1000.0f;

}

void App::InitializeBricks()
{
	sizeOfBricks = Vector2f(window.getSize().x / NUM_OF_BRICKS_ROWS, (window.getSize().y / 2) / NUM_OF_BRICK_COLUMNS);
	
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
	
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			bricks[i][j].setSize(sizeOfBricks);		

			bricks[i][j].setPosition((float)(window.getSize().x / NUM_OF_BRICK_COLUMNS) *  j, (float)((window.getSize().y / 2) / NUM_OF_BRICKS_ROWS) * i);		
			bricks[i][j].setOutlineThickness(5);
			bricks[i][j].setOutlineColor(Color::Black);
			bricks[i][j].setFillColor(Color::Blue);
			collided[i][j] = false;
		}
	}
}

void App::CircleMovement()
{		
	ball.move(speed * deltaTime);
	// Border Collision Right
	if (ball.getPosition().x > (window.getSize().x - radius))
	{
		ball.setPosition(window.getSize().x - radius, ball.getPosition().y);		
		speed.x *= -1;
	}

	// Border Collision Left
	if (ball.getPosition().x < radius)
	{
		ball.setPosition(radius, ball.getPosition().y);		
		speed.x *= -1;
	}

	//Border Collision Bottom
	if (ball.getPosition().y > (window.getSize().y - radius))
	{
		ball.setPosition(ball.getPosition().x, window.getSize().y - radius);		
		speed.y *= -1;
	}

	// Border Collision Top
	if (ball.getPosition().y < radius)
	{
		ball.setPosition(ball.getPosition().x, radius);		
		speed.y *= -1;
	}	

	BrickCollision();
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
	if (paddle.getPosition().x > (window.getSize().x - paddleSize.x)) //Right
	{
		paddle.setPosition(window.getSize().x - paddleSize.x, paddle.getPosition().y);
	}
	if (paddle.getPosition().x < 0) // Left
	{
		paddle.setPosition(0, paddle.getPosition().y);
	}
}

void App::PaddleCollision()
{
	if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds()))
	{
		//left
		if (ball.getPosition().x < paddle.getPosition().x)
		{
			speed.x *= -1;
			ball.setPosition(paddle.getPosition().x - (radius + 1), ball.getPosition().y);
		}
		//right
		else if (ball.getPosition().x > paddle.getPosition().x + paddleSize.x)
		{
			speed.x *= -1;
			ball.setPosition((paddle.getPosition().x + paddleSize.x) + (radius + 1), ball.getPosition().y);
		}
		//top
		else if (ball.getPosition().y < paddle.getPosition().y)
		{
			speed.y *= -1;
			ball.setPosition(ball.getPosition().x, (paddle.getPosition().y) - (radius + 1));
		}
		//bottom
		else if (ball.getPosition().y > paddle.getPosition().y + paddleSize.y)
		{
			speed.y *= -1;
			ball.setPosition(ball.getPosition().x, (paddle.getPosition().y + paddleSize.y) + (radius + 1));
		}
	}
}

void App::BrickCollision()
{
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; i++)
		{
			if (ball.getGlobalBounds().intersects(bricks[i][j].getGlobalBounds()))
			{
				//left
				if (ball.getPosition().x < bricks[i][j].getPosition().x)
				{
					speed.x *= -1;
					ball.setPosition(bricks[i][j].getPosition().x - (radius + 1), ball.getPosition().y);
				}
				//right
				else if (ball.getPosition().x > bricks[i][j].getPosition().x + sizeOfBricks.x)
				{
					speed.x *= -1;
					ball.setPosition((bricks[i][j].getPosition().x + sizeOfBricks.x) + (radius + 1), ball.getPosition().y);
				}
				//top
				else if (ball.getPosition().y < bricks[i][j].getPosition().y)
				{
					speed.y *= -1;
					ball.setPosition(ball.getPosition().x, (bricks[i][j].getPosition().y) - (radius + 1));
				}
				//bottom
				else if (ball.getPosition().y > bricks[i][j].getPosition().y + sizeOfBricks.y)
				{
					speed.y *= -1;
					ball.setPosition(ball.getPosition().x, (bricks[i][j].getPosition().y + sizeOfBricks.y) + (radius + 1));
				}
				collided[i][j] = true;
			}
		}
	}
	
}



