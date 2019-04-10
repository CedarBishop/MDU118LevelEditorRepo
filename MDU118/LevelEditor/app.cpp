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
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		delete[] brickPtrs[i];
		delete[] brickShadowPtrs[i];
	}
	delete[] brickPtrs;
	delete[] brickShadowPtrs;
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
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			if (hasStarted == false)
			{
				//window.draw(brickShadows[i][j]);
				window.draw(brickShadowPtrs[i][j]);
			}
		}
	}
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			if (collided[i][j] == false)
			{
				//window.draw(bricks[i][j]);
				window.draw(brickPtrs[i][j]);
			}			
		}
	}
	if (hasStarted == false)
	{
		window.draw(instructionsText);
		window.draw(button);
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
		for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
		{
			for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
			{
				if (brickPtrs[i][j].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
				{
					collided[i][j] = !collided[i][j];
				}
			}
		}	
		if (button.getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			hasStarted = true;
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
	brickPtrs = new RectangleShape *[NUM_OF_BRICKS_ROWS];
	brickShadowPtrs = new RectangleShape *[NUM_OF_BRICKS_ROWS];
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		brickPtrs[i] = new RectangleShape[NUM_OF_BRICK_COLUMNS];
		brickShadowPtrs[i] = new RectangleShape[NUM_OF_BRICK_COLUMNS];
	}
	image.loadFromFile("Images/Goku.png");
	sizeOfBricks = Vector2f(backgroundSprite.getGlobalBounds().width / (NUM_OF_BRICKS_ROWS * 2), (backgroundSprite.getGlobalBounds().height / 2) / (NUM_OF_BRICK_COLUMNS * 2));
	brickTexture.loadFromFile("Images/BlockTexture.png");
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{	
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			color[i][j] = image.getPixel((image.getSize().x / NUM_OF_BRICK_COLUMNS) * j + 1, ((image.getSize().y / NUM_OF_BRICKS_ROWS) * i + 1));
			
			brickPtrs[i][j].setSize(sizeOfBricks);
			brickPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / NUM_OF_BRICK_COLUMNS) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / NUM_OF_BRICKS_ROWS) * i) + (sizeOfBricks.y / 2));
			brickPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickPtrs[i][j].setFillColor(color[i][j]);
			brickPtrs[i][j].setOutlineColor(Color::Black);

			brickShadowPtrs[i][j].setSize(sizeOfBricks);
			brickShadowPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / NUM_OF_BRICK_COLUMNS) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / NUM_OF_BRICKS_ROWS) * i) + (sizeOfBricks.y / 2));
			brickShadowPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickShadowPtrs[i][j].setOutlineColor(Color::Black);
			brickShadowPtrs[i][j].setFillColor(Color::Transparent);

			//bricks[i][j].setSize(sizeOfBricks);	
			//bricks[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / NUM_OF_BRICK_COLUMNS) *  j) + (sizeOfBricks.x /2) + sideBarRatio,(((window.getSize().y / 2) / NUM_OF_BRICKS_ROWS) * i) + (sizeOfBricks.y / 2));		
			//bricks[i][j].setOutlineThickness(window.getSize().y / 200);
			////bricks[i][j].setTexture(&brickTexture);
			//bricks[i][j].setFillColor(color[i][j]);
			//bricks[i][j].setOutlineColor(Color::Black);
			
			/*brickShadows[i][j].setSize(sizeOfBricks);
			brickShadows[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / NUM_OF_BRICK_COLUMNS) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / NUM_OF_BRICKS_ROWS) * i) + (sizeOfBricks.y / 2));
			brickShadows[i][j].setOutlineThickness(window.getSize().y / 200);
			brickShadows[i][j].setOutlineColor(Color::Black);
			brickShadows[i][j].setFillColor(Color::Transparent);*/

			collided[i][j] = (color[i][j].a > 10) ? false : true;
			startingBrickStatus[i][j] = collided[i][j];
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
	buttonSize = Vector2f(window.getSize().x / 8, window.getSize().y / 16);
	button.setPosition(window.getSize().x / 2 - (buttonSize.x / 2), window.getSize().y / 1.5f);
	button.setSize(buttonSize);
	button.setOutlineThickness(window.getSize().y / 200);
	button.setOutlineColor(Color::Black);
	button.setFillColor(Color::Red);
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
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			if (ball.getGlobalBounds().intersects(brickPtrs[i][j].getGlobalBounds()) && collided[i][j] == false)
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
				collided[i][j] = true;
				TestGameWin();
			}			
		}
	}	
}
void App::TestGameWin()
{
	int sumOfBools = 0;
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			if (collided[i][j])
			{
				sumOfBools++;
			}
		}
	}

	if (hasStarted && sumOfBools == (NUM_OF_BRICKS_ROWS * NUM_OF_BRICK_COLUMNS))
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
	for (int i = 0; i < NUM_OF_BRICKS_ROWS; i++)
	{
		for (int j = 0; j < NUM_OF_BRICK_COLUMNS; j++)
		{
			collided[i][j] = startingBrickStatus[i][j];
		}
	}
}