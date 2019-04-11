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
	for (int i = 0; i < currentGridSize; i++)
	{
		delete[] brickPtrs[i];
		delete[] brickShadowPtrs[i];
		delete[] colorPtrs[i];
		delete[] collidedPtrs[i];
		delete[] startingBrickStatusPtrs[i];
	}
	delete[] brickPtrs;
	delete[] brickShadowPtrs;
	delete[] colorPtrs;
	delete[] collidedPtrs;
	delete[] startingBrickStatusPtrs;
}


// Initialise member variables here
bool App::Init()
{	
	srand(time(NULL));
	hasStarted = false;
	InitializeBackGround();
	InitializeCircles();	
	InitializePaddle();
	InitializeBricks();
	InitializeText();
	InitializeSound();
	InitializeButton();

	return true;
}



// Add game logic here
void App::Update()
{
	deltaTime = clock.restart().asSeconds();
	CircleMovement();
	PaddleMovement();
	PaddleCollision();
	BrickCollision();
}

// Rendering
void App::Draw()
{
	window.clear();
	window.setView(view);

	// Sprite Drawing Starts here 

	window.draw(backgroundSprite);
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			if (hasStarted == false)
			{
				window.draw(brickShadowPtrs[i][j]);
			}
		}
	}
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			if (collidedPtrs[i][j] == false)
			{
				window.draw(brickPtrs[i][j]);
			}			
		}
	}
	if (hasStarted == false)
	{
		window.draw(instructionsText);
		for (int i = 0; i < 7; i++)
		{
			window.draw(button[i]);
		}		
		window.draw(buttonText);
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
		for (int i = 0; i < currentGridSize; i++)
		{
			for (int j = 0; j < currentGridSize; j++)
			{
				if (brickPtrs[i][j].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
				{
					collidedPtrs[i][j] = !collidedPtrs[i][j];
				}
			}
		}	
		if (button[0].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			hasStarted = true;
		}
		for (int i = 1; i < 7; i++)
		{
			if (button[i].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
			{
				ResizeArrays(5 * i);
			}
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
	radius = window.getSize().x * 0.01f;
	ballTexture.loadFromFile("Images/TennisBall.png");	

	ball.setRadius(radius);
	ball.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 1.22f));
	ball.setOrigin(Vector2f(radius, radius));
	ball.setTexture(&ballTexture);		

	sign = 2 * (rand() % 2) - 1;
	speed = Vector2f(sign * (rand() % 201 + 300),-(rand() % 201 + 300));
}

void App::InitializePaddle()
{
	paddleSize = Vector2f(0.1f * window.getSize().x, 0.05f * window.getSize().y);
	paddleStartingPosition = Vector2f((window.getSize().x / 2) - (paddleSize.x / 2), window.getSize().y * 0.85f);
	paddle.setSize(paddleSize);
	paddle.setPosition(paddleStartingPosition);
	paddleTexture.loadFromFile("Images/PaddleTexture.jpg");
	paddle.setTexture(&paddleTexture);
	paddleVelocity = 1000.0f;

}

void App::InitializeBricks()
{
	brickPtrs = new RectangleShape *[30];
	brickShadowPtrs = new RectangleShape *[30];
	colorPtrs = new Color*[30];
	collidedPtrs = new bool*[30];
	startingBrickStatusPtrs = new bool*[30];
	for (int i = 0; i < 30; i++)
	{
		brickPtrs[i] = new RectangleShape[30];
		brickShadowPtrs[i] = new RectangleShape[30];
		colorPtrs[i] = new Color[30];
		collidedPtrs[i] = new bool[30];
		startingBrickStatusPtrs[i] = new bool[30];
	}
	currentGridSize = 30;
	image.loadFromFile("Images/Goku.png");
	brickTexture.loadFromFile("Images/BlockTexture.png");
	sizeOfBricks = Vector2f(backgroundSprite.getGlobalBounds().width / (currentGridSize * 2), (backgroundSprite.getGlobalBounds().height / 2) / (currentGridSize * 2));
	for (int i = 0; i < currentGridSize; i++)
	{	
		for (int j = 0; j < currentGridSize; j++)
		{
			colorPtrs[i][j] = image.getPixel((image.getSize().x / currentGridSize) * j + 1, ((image.getSize().y / currentGridSize) * i + 1));
			
			brickPtrs[i][j].setSize(sizeOfBricks);
			brickPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / currentGridSize) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / currentGridSize) * i) + (sizeOfBricks.y / 2));
			brickPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickPtrs[i][j].setFillColor(colorPtrs[i][j]);
			brickPtrs[i][j].setOutlineColor(Color::Black);

			brickShadowPtrs[i][j].setSize(sizeOfBricks);
			brickShadowPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / currentGridSize) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / currentGridSize) * i) + (sizeOfBricks.y / 2));
			brickShadowPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickShadowPtrs[i][j].setOutlineColor(Color::Black);
			brickShadowPtrs[i][j].setFillColor(Color::Transparent);

			collidedPtrs[i][j] = (colorPtrs[i][j].a > 10) ? false : true;
			startingBrickStatusPtrs[i][j] = collidedPtrs[i][j];
		}
	}
}

void App::InitializeText()
{
	instructionsFont.loadFromFile("Fonts/Natural Marker.ttf");
	instructionsText.setFont(instructionsFont);
	instructionsText.setPosition(window.getSize().x /3.5f, window.getSize().y / 2);
	instructionsText.setString("Click Empty Blocks To Create Bricks");
	instructionsText.setCharacterSize(window.getSize().y / 25);
	buttonText.setFont(instructionsFont);
	buttonText.setPosition(window.getSize().x / 2.1f, window.getSize().y / 1.48f);
	buttonText.setString("Play");
	buttonText.setCharacterSize(window.getSize().y / 25);
}

void App::InitializeSound()
{
	collisionBuffer.loadFromFile("Sound/collision.wav");
	collisionSound.setBuffer(collisionBuffer);
	gameOverBuffer.loadFromFile("Sound/gameOver.wav");
	gameOverSound.setBuffer(gameOverBuffer);
	gameWinBuffer.loadFromFile("Sound/gameWin.wav");
	gameWinSound.setBuffer(gameWinBuffer);
	BGMusic.openFromFile("Sound/backGroundMusic.wav");
	BGMusic.play();
	BGMusic.setLoop(true);
}

void App::InitializeButton()
{
	buttonSize = Vector2f(window.getSize().x / 10, window.getSize().y / 20);
	button[0].setPosition(window.getSize().x / 2 - (buttonSize.x / 2), window.getSize().y / 1.5f);
	for (int i = 0; i < 7; i++)
	{
		button[i].setSize(buttonSize);
		button[i].setOutlineThickness(window.getSize().y / 200);
		button[i].setOutlineColor(Color::Black);
		button[i].setFillColor(Color::Red);
	}
	for (int i = 1; i < 7; i++)
	{
		button[i].setPosition(window.getSize().x * 0.875, window.getSize().y * (0.1f * (float)i));
	}

	
}

void App::InitializeBackGround()
{
	sideBarRatio = window.getSize().x * 0.15f;
	backgroundTexture.loadFromFile("Images/Brick.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(((float)window.getSize().x / backgroundTexture.getSize().x) * 0.7f, (float)window.getSize().y / backgroundTexture.getSize().y);
	backgroundSprite.setPosition(sideBarRatio,0);
}

void App::CircleMovement()
{	
	if (hasStarted)
	{
		ball.move(speed * deltaTime);		
	}
	
	// Border Collision Right
	if (ball.getPosition().x > ((backgroundSprite.getGlobalBounds().width + sideBarRatio) - radius))
	{
		ball.setPosition((backgroundSprite.getGlobalBounds().width + sideBarRatio) - radius, ball.getPosition().y);		
		speed.x *= -1;
		collisionSound.play();
	}

	// Border Collision Left
	if (ball.getPosition().x < radius + sideBarRatio)
	{
		ball.setPosition(radius + sideBarRatio, ball.getPosition().y);		
		speed.x *= -1;
		collisionSound.play();
	}

	//Border Collision Bottom
	if (ball.getPosition().y > (window.getSize().y - radius))
	{
		/*ball.setPosition(ball.getPosition().x, window.getSize().y - radius);		
		speed.y *= -1;*/
		gameOverSound.play();
		ResetGame();
	}

	// Border Collision Top
	if (ball.getPosition().y < radius)
	{
		ball.setPosition(ball.getPosition().x, radius);		
		speed.y *= -1;
		collisionSound.play();
	}	
}

void App::PaddleMovement()
{
	if (Keyboard::isKeyPressed(Keyboard::Left) && hasStarted)
	{
		paddle.move(-paddleVelocity * deltaTime, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) && hasStarted)
	{
		paddle.move(paddleVelocity * deltaTime, 0);
	}

	//Player Border Collision
	if (paddle.getPosition().x > ((backgroundSprite.getGlobalBounds().width + sideBarRatio) - paddleSize.x)) //Right
	{
		paddle.setPosition(((backgroundSprite.getGlobalBounds().width + sideBarRatio) - paddleSize.x), paddle.getPosition().y);
	}
	if (paddle.getPosition().x < sideBarRatio) // Left
	{
		paddle.setPosition(sideBarRatio, paddle.getPosition().y);
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
		collisionSound.play();
	}
}

void App::BrickCollision()
{
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			if (ball.getGlobalBounds().intersects(brickPtrs[i][j].getGlobalBounds()) && collidedPtrs[i][j] == false)
			{
				//left
				if (ball.getPosition().x < brickPtrs[i][j].getPosition().x)
				{
					speed.x *= -1;
					ball.setPosition(brickPtrs[i][j].getPosition().x - (radius + 1), ball.getPosition().y);
				}
				//right
				else if (ball.getPosition().x > brickPtrs[i][j].getPosition().x + sizeOfBricks.x)
				{
					speed.x *= -1;
					ball.setPosition((brickPtrs[i][j].getPosition().x + sizeOfBricks.x) + (radius + 1), ball.getPosition().y);
				}
				//top
				else if (ball.getPosition().y < brickPtrs[i][j].getPosition().y)
				{
					speed.y *= -1;
					ball.setPosition(ball.getPosition().x, (brickPtrs[i][j].getPosition().y) - (radius + 1));
				}
				//bottom
				else if (ball.getPosition().y > brickPtrs[i][j].getPosition().y + sizeOfBricks.y)
				{
					speed.y *= -1;
					ball.setPosition(ball.getPosition().x, (brickPtrs[i][j].getPosition().y + sizeOfBricks.y) + (radius + 1));
				}
				collisionSound.play();
				collidedPtrs[i][j] = true;
				TestGameWin();
			}			
		}
	}	
}
void App::TestGameWin()
{
	int sumOfBools = 0;
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			if (collidedPtrs[i][j])
			{
				sumOfBools++;
			}
		}
	}

	if (hasStarted && sumOfBools == (currentGridSize * currentGridSize))
	{
		gameWinSound.play();
		ResetGame();
	}
}

void App::ResetGame()
{
	hasStarted = false;
	ball.setPosition(window.getSize().x / 2, window.getSize().y / 1.22f);
	speed = Vector2f(sign * (rand() % 201 + 300), -(rand() % 201 + 300));
	paddle.setPosition(paddleStartingPosition);
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			collidedPtrs[i][j] = startingBrickStatusPtrs[i][j];
		}
	}
}

void App::ResizeArrays(int gridSize)
{
	if (currentGridSize == gridSize)
	{
		return;
	}
	for (int i = 0; i < currentGridSize; i++)
	{
		delete[] brickPtrs[i];
		delete[] brickShadowPtrs[i];
		delete[] colorPtrs[i];
		delete[] collidedPtrs[i];
		delete[] startingBrickStatusPtrs[i];
	}
	delete[] brickPtrs;
	delete[] brickShadowPtrs;
	delete[] colorPtrs;
	delete[] collidedPtrs;
	delete[] startingBrickStatusPtrs;

	brickPtrs = new RectangleShape *[gridSize];
	brickShadowPtrs = new RectangleShape *[gridSize];
	colorPtrs = new Color*[gridSize];
	collidedPtrs = new bool*[gridSize];
	startingBrickStatusPtrs = new bool*[gridSize];
	for (int i = 0; i < gridSize; i++)
	{
		brickPtrs[i] = new RectangleShape[gridSize];
		brickShadowPtrs[i] = new RectangleShape[gridSize];
		colorPtrs[i] = new Color[gridSize];
		collidedPtrs[i] = new bool[gridSize];
		startingBrickStatusPtrs[i] = new bool[gridSize];
	}
	currentGridSize = gridSize;
	brickTexture.loadFromFile("Images/BlockTexture.png");
	sizeOfBricks = Vector2f(backgroundSprite.getGlobalBounds().width / (currentGridSize * 2), (backgroundSprite.getGlobalBounds().height / 2) / (currentGridSize * 2));
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			colorPtrs[i][j] = image.getPixel((image.getSize().x / currentGridSize) * j + 1, ((image.getSize().y / currentGridSize) * i + 1));

			brickPtrs[i][j].setSize(sizeOfBricks);
			brickPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / currentGridSize) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / currentGridSize) * i) + (sizeOfBricks.y / 2));
			brickPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickPtrs[i][j].setFillColor(colorPtrs[i][j]);
			brickPtrs[i][j].setOutlineColor(Color::Black);

			brickShadowPtrs[i][j].setSize(sizeOfBricks);
			brickShadowPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / currentGridSize) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / currentGridSize) * i) + (sizeOfBricks.y / 2));
			brickShadowPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickShadowPtrs[i][j].setOutlineColor(Color::Black);
			brickShadowPtrs[i][j].setFillColor(Color::Transparent);

			collidedPtrs[i][j] = (colorPtrs[i][j].a > 10) ? false : true;
			startingBrickStatusPtrs[i][j] = collidedPtrs[i][j];
		}
	}
}
