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
	// Deleting all 2D dynamic arrays
	for (int i = 0; i < currentGridSize; i++)
	{
		delete[] brickPtrs[i];
		delete[] colorPtrs[i];
		delete[] collidedPtrs[i];
		delete[] startingBrickStatusPtrs[i];
	}
	delete[] brickPtrs;
	delete[] colorPtrs;
	delete[] collidedPtrs;
	delete[] startingBrickStatusPtrs;
}

// Initialise member variables here
bool App::Init()
{	
	srand(time(NULL));
	hasStarted = false;
	acceptInput = true;
	InitializeBackGround();
	InitializeCircles();	
	InitializePaddle();
	InitializeBricks();
	InitializeText();
	InitializeSound();
	InitializeButton();
	InitialiseDisplayImages();

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
	for (int i = 0; i < 3; i++)
	{
		if (currentImageIndex == i && hasStarted == false && isDisplaying)
		{
			window.draw(displaySprite[i]);
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
		for (int i = 0; i < 16; i++)
		{
			window.draw(button[i]);
			window.draw(buttonText[i]);
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

	if (event.type == Event::MouseButtonReleased)
	{
		acceptInput = true;
	}
	ButtonHandling();
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


// creates the bricks and puts its pixel coirdinate in retrospect to the image pixel coirdinate and puts that pixel colour in the brick
void App::InitializeBricks()
{
	brickPtrs = new RectangleShape *[30];
	colorPtrs = new Color*[30];
	collidedPtrs = new bool*[30];
	startingBrickStatusPtrs = new bool*[30];
	for (int i = 0; i < 30; i++)
	{
		brickPtrs[i] = new RectangleShape[30];
		colorPtrs[i] = new Color[30];
		collidedPtrs[i] = new bool[30];
		startingBrickStatusPtrs[i] = new bool[30];
	}
	currentGridSize = 30;
	minColorValue = Color(0,0,0,1);
	maxColorValue = Color(255,255,255,255);
	image[0].loadFromFile("Images/Goku.png");
	image[1].loadFromFile("Images/Forest.jpg");
	image[2].loadFromFile("Images/Sunset.jpg");
	currentImageIndex = 0;
	brickTexture.loadFromFile("Images/BlockTexture.png");
	sizeOfBricks = Vector2f(backgroundSprite.getGlobalBounds().width / (currentGridSize * 2), (backgroundSprite.getGlobalBounds().height / 2) / (currentGridSize * 2));
	for (int i = 0; i < currentGridSize; i++)
	{	
		for (int j = 0; j < currentGridSize; j++)
		{
			colorPtrs[i][j] = image[currentImageIndex].getPixel((image[currentImageIndex].getSize().x / currentGridSize) * j + 1, ((image[currentImageIndex].getSize().y / currentGridSize) * i + 1));
			
			brickPtrs[i][j].setSize(sizeOfBricks);
			brickPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / currentGridSize) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / currentGridSize) * i) + (sizeOfBricks.y / 2));
			brickPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickPtrs[i][j].setFillColor(colorPtrs[i][j]);
			brickPtrs[i][j].setOutlineColor(Color::Black);

			collidedPtrs[i][j] = ((colorPtrs[i][j].r >= minColorValue.r && colorPtrs[i][j].r <= maxColorValue.r) && (colorPtrs[i][j].g >= minColorValue.g && colorPtrs[i][j].g <= maxColorValue.g) && (colorPtrs[i][j].b >= minColorValue.b && colorPtrs[i][j].b <= maxColorValue.b) && (colorPtrs[i][j].a >= minColorValue.a && colorPtrs[i][j].b <= maxColorValue.b)) ? false : true;
			startingBrickStatusPtrs[i][j] = collidedPtrs[i][j];			
		}
	}
}

void App::InitializeText()
{
	font.loadFromFile("Fonts/Natural Marker.ttf");
	instructionsText.setFont(font);
	instructionsText.setPosition(window.getSize().x /3.5f, window.getSize().y / 1.8f);
	instructionsText.setString("Click Empty Blocks To Create Bricks");
	instructionsText.setCharacterSize(window.getSize().y / 25);
	for (int i = 0; i < 16; i++)
	{
		buttonText[i].setFont(font);
		buttonText[i].setCharacterSize(window.getSize().y / 25);
	}
	buttonText[0].setFillColor(Color::Red);
	buttonText[0].setPosition(window.getSize().x * 0.03f, window.getSize().y * 0.1f);
	buttonText[0].setString("Play");
	buttonText[1].setString("5 x 5");
	buttonText[2].setString("10 x 10");
	buttonText[3].setString("15 x 15");
	buttonText[4].setString("20 x 20");
	buttonText[5].setString("25 x 25");
	buttonText[6].setString("30 x 30");
	buttonText[7].setString("Red");
	buttonText[8].setString("Green");
	buttonText[9].setString("Blue");
	buttonText[10].setString("Alpha");
	buttonText[11].setString("Image 1");
	buttonText[12].setString("Image 2");
	buttonText[13].setString("Image 3");
	buttonText[14].setString("Display");
	buttonText[15].setString("Random");
	for (int i = 1; i < 7; i++)
	{
		buttonText[i].setPosition(window.getSize().x * 0.875f, window.getSize().y * (0.1f * i));	
	}
	float j = 2;
	for (int i = 7; i < 14; i++)
	{
		buttonText[i].setPosition(window.getSize().x * 0.025f, window.getSize().y * (0.1f * j));
		j++;
	}
	buttonText[14].setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.7f);
	buttonText[15].setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.8f);
}

void App::InitializeSound()
{
	brickCollisionBuffer.loadFromFile("Sound/collision.wav");
	brickCollisionSound.setBuffer(brickCollisionBuffer);
	paddleCollisionBuffer.loadFromFile("Sound/paddleCollision.wav");
	paddleCollisionSound.setBuffer(paddleCollisionBuffer);
	borderCollisionBuffer.loadFromFile("Sound/borderCollision.wav");
	borderCollisionSound.setBuffer(borderCollisionBuffer);
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
	button[0].setPosition(window.getSize().x * 0.025f, window.getSize().y * 0.1f);
	for (int i = 0; i < 16; i++)
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
	float j = 2;
	for (int i = 7; i < 14; i++)
	{
		button[i].setPosition(window.getSize().x * 0.025, window.getSize().y * (0.1f * j));
		j++;
	}
	button[0].setFillColor(Color::White);
	button[14].setPosition(window.getSize().x * 0.875f,window.getSize().y * 0.7f);
	button[15].setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.8f);
}

void App::InitializeBackGround()
{
	sideBarRatio = window.getSize().x * 0.15f;
	backgroundTexture.loadFromFile("Images/Brick.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(((float)window.getSize().x / backgroundTexture.getSize().x) * 0.7f, (float)window.getSize().y / backgroundTexture.getSize().y);
	backgroundSprite.setPosition(sideBarRatio,0);
}

// handles the speed of the bal and its collisions with the borders
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
		borderCollisionSound.play();
	}

	// Border Collision Left
	if (ball.getPosition().x < radius + sideBarRatio)
	{
		ball.setPosition(radius + sideBarRatio, ball.getPosition().y);		
		speed.x *= -1;
		borderCollisionSound.play();
	}

	//Border Collision Bottom
	if (ball.getPosition().y > (window.getSize().y - radius))
	{
		gameOverSound.play();
		ResetGame();
	}

	// Border Collision Top
	if (ball.getPosition().y < radius)
	{
		ball.setPosition(ball.getPosition().x, radius);		
		speed.y *= -1;
		borderCollisionSound.play();
	}	
}


//Stops the paddle from going out of bounds
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

// Determines if the ball hits the padlle
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
		paddleCollisionSound.play();
	}
}

// detects collision of the bricks then turns them off after they are hit
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
				brickCollisionSound.play();
				collidedPtrs[i][j] = true;
				TestGameWin();
			}			
		}
	}	
}

//Checks if all bricks are off if so reset game
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

//Resets the game after winning or losing
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

// deleltes and recreates all the 2d dynamic arrays to the new grid size then reecreates bricks to the new sizes and positions.
void App::ResizeArrays(int gridSize)
{
	if (currentGridSize == gridSize)
	{
		return;
	}
	for (int i = 0; i < currentGridSize; i++)
	{
		delete[] brickPtrs[i];
		delete[] colorPtrs[i];
		delete[] collidedPtrs[i];
		delete[] startingBrickStatusPtrs[i];
	}
	delete[] brickPtrs;
	delete[] colorPtrs;
	delete[] collidedPtrs;
	delete[] startingBrickStatusPtrs;

	brickPtrs = new RectangleShape *[gridSize];
	colorPtrs = new Color*[gridSize];
	collidedPtrs = new bool*[gridSize];
	startingBrickStatusPtrs = new bool*[gridSize];
	for (int i = 0; i < gridSize; i++)
	{
		brickPtrs[i] = new RectangleShape[gridSize];
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
			colorPtrs[i][j] = image[currentImageIndex].getPixel((image[currentImageIndex].getSize().x / currentGridSize) * j + 1, ((image[currentImageIndex].getSize().y / currentGridSize) * i + 1));

			brickPtrs[i][j].setSize(sizeOfBricks);
			brickPtrs[i][j].setPosition(((backgroundSprite.getGlobalBounds().width / currentGridSize) *  j) + (sizeOfBricks.x / 2) + sideBarRatio, (((window.getSize().y / 2) / currentGridSize) * i) + (sizeOfBricks.y / 2));
			brickPtrs[i][j].setOutlineThickness(window.getSize().y / 200);
			brickPtrs[i][j].setFillColor(colorPtrs[i][j]);
			brickPtrs[i][j].setOutlineColor(Color::Black);

			collidedPtrs[i][j] = ((colorPtrs[i][j].r >= minColorValue.r && colorPtrs[i][j].r <= maxColorValue.r) && (colorPtrs[i][j].g >= minColorValue.g && colorPtrs[i][j].g <= maxColorValue.g) && (colorPtrs[i][j].b >= minColorValue.b && colorPtrs[i][j].b <= maxColorValue.b) && (colorPtrs[i][j].a >= minColorValue.a && colorPtrs[i][j].b <= maxColorValue.b)) ? false : true;
			startingBrickStatusPtrs[i][j] = collidedPtrs[i][j];
		}
	}
}


//Changes the dominate colour of the bricks to be turned on
void App::changeDominateColor(int minRed, int minGreen, int minBlue, int minAlpha, int maxRed, int maxGreen, int maxBlue, int maxAlpha)
{
	if (minColorValue.r == minRed && minColorValue.g == minGreen && minColorValue.b == minBlue && minColorValue.a == minAlpha)
	{
		return;
	}
	minColorValue = Color(minRed,minGreen,minBlue,minAlpha);
	maxColorValue = Color(maxRed,maxGreen,maxBlue,maxAlpha);
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			collidedPtrs[i][j] = ((colorPtrs[i][j].r >= minColorValue.r && colorPtrs[i][j].r <= maxColorValue.r) && (colorPtrs[i][j].g >= minColorValue.g && colorPtrs[i][j].g <= maxColorValue.g)  && (colorPtrs[i][j].b >= minColorValue.b && colorPtrs[i][j].b <= maxColorValue.b) && (colorPtrs[i][j].a >= minColorValue.a && colorPtrs[i][j].b <= maxColorValue.b)) ? false : true;
			startingBrickStatusPtrs[i][j] = collidedPtrs[i][j];
		}
	}
}


//Changes between the 3 set images
void App::changeImage(int newImageIndex)
{
	if (currentImageIndex == newImageIndex)
	{
		return;
	}
	currentImageIndex = newImageIndex;
	for (int i = 0; i < currentGridSize; i++)
	{
		for (int j = 0; j < currentGridSize; j++)
		{
			colorPtrs[i][j] = image[currentImageIndex].getPixel((image[currentImageIndex].getSize().x / currentGridSize) * j + 1, ((image[currentImageIndex].getSize().y / currentGridSize) * i + 1));
			brickPtrs[i][j].setFillColor(colorPtrs[i][j]);
			collidedPtrs[i][j] = ((colorPtrs[i][j].r >= minColorValue.r && colorPtrs[i][j].r <= maxColorValue.r) && (colorPtrs[i][j].g >= minColorValue.g && colorPtrs[i][j].g <= maxColorValue.g) && (colorPtrs[i][j].b >= minColorValue.b && colorPtrs[i][j].b <= maxColorValue.b) && (colorPtrs[i][j].a >= minColorValue.a && colorPtrs[i][j].b <= maxColorValue.b)) ? false : true;
			startingBrickStatusPtrs[i][j] = collidedPtrs[i][j];		
		}
	}
}

//Creates a sprite of the image s that gets displayed behind the bricks
void App::InitialiseDisplayImages()
{
	for (int i = 0; i < 3; i++)
	{
		displayTexture[i].loadFromImage(image[i]);
		displaySprite[i].setTexture(displayTexture[i]);
		displaySprite[i].setScale(((float)window.getSize().x / image[i].getSize().x) * 0.7f, ((float)window.getSize().y / image[i].getSize().y) * 0.5f);
		displaySprite[i].setPosition(sideBarRatio,0);
	}
	isDisplaying = true;
}

//creates a random minimum and maximum of range of colour bricks to be turned on
void App::RandomRGB()
{	
	randomMinColor.r = rand() % 128;
	randomMinColor.g = rand() % 128;
	randomMinColor.b = rand() % 128;
	randomMinColor.a = 100;
	randomMaxColor.r = rand() % 128 + 128;
	randomMaxColor.g = rand() % 128 + 128;
	randomMaxColor.b = rand() % 128 + 128;
	randomMaxColor.a = 100;
}

//handles all button clicks
void App::ButtonHandling()
{
	Vector2i localMousePosition = Mouse::getPosition(window);
	if (Mouse::isButtonPressed(Mouse::Left) && acceptInput)
	{
		acceptInput = false;
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
			TestGameWin();
		}
		for (int i = 1; i < 7; i++)
		{
			if (button[i].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
			{
				ResizeArrays(5 * i);
			}
		}
		if (button[7].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeDominateColor(100, 0, 0, 100, 255, 100, 100, 100);
		}
		if (button[8].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeDominateColor(0, 100, 0, 100, 100, 255, 100, 100);
		}
		if (button[9].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeDominateColor(0, 0, 100, 100, 100, 100, 255, 100);
		}
		if (button[10].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeDominateColor(0, 0, 0, 100, 100, 100, 255, 100);
		}
		if (button[11].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeImage(0);
		}
		if (button[12].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeImage(1);
		}
		if (button[13].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			changeImage(2);
		}
		if (button[14].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			isDisplaying = !isDisplaying;
		}
		if (button[15].getGlobalBounds().contains(Vector2f(localMousePosition)) && hasStarted == false)
		{
			RandomRGB();
			changeDominateColor(randomMinColor.r, randomMinColor.g, randomMinColor.b, randomMinColor.a, randomMaxColor.r, randomMaxColor.g, randomMaxColor.b, randomMaxColor.a);
		}
	}
}