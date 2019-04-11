#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
using namespace sf;

//
//const int GRID_OF_FIVE = 5;
//const int GRID_OF_TEN = 10;
//const int GRID_OF_FIFTEEN = 15;
//const int GRID_OF_TWENTY = 20;
//const int GRID_OF_TWENTYFIVE = 25;
//const int GRID_OF_THIRTY = 30;


class App
{
	//Game Logic variables
	Clock clock;
	Event event;
	View view;
	RenderWindow window;
	float deltaTime;
	bool hasStarted;
	//	

	//Background variables

	Texture backgroundTexture;
	Sprite backgroundSprite;
	float sideBarRatio;

	//

	//Ball variables
	CircleShape ball;
	float radius;
	Texture ballTexture;
	int sign;
	int sign2;
	Vector2f speed;
	//

	//Paddle variables

	RectangleShape paddle;
	Texture paddleTexture;
	float paddleVelocity;
	Vector2f paddleStartingPosition;
	Vector2f paddleSize;
	
	//

	// Bricks

	Vector2f sizeOfBricks;
	Texture brickTexture;
	RectangleShape** brickPtrs;
	RectangleShape** brickShadowPtrs;
	bool** collidedPtrs;
	bool** startingBrickStatusPtrs;
	int currentGridSize;
	int GRID_NUMBERS[6];
	// Texts

	Text instructionsText;
	Font instructionsFont;
	Text buttonText;
	//

	// SFX
	SoundBuffer collisionBuffer;
	Sound collisionSound;

	SoundBuffer gameOverBuffer;
	Sound gameOverSound;

	SoundBuffer gameWinBuffer;
	Sound gameWinSound;

	Music BGMusic;

	//

	// Button
	RectangleShape button[7];
	Vector2f buttonSize;	
		
	//

	// Image Analyser
	Image image;
	Color** colorPtrs;
	// Methods

public:
	App(const char*, int, int, int);
	~App();
	bool Init();
	void HandleEvents();
	void Update();
	void Draw();
	void Run();

private:
	void InitializeCircles();
	void InitializePaddle();
	void InitializeBricks();
	void CircleMovement();
	void PaddleMovement();
	void PaddleCollision();
	void BrickCollision();
	void TestGameWin();
	void ResetGame();
	void InitializeText();
	void InitializeSound();
	void InitializeButton();
	void InitializeBackGround();
	void ResizeArrays(int);	
	//
};

