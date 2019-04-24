#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
using namespace sf;

class App
{
	//Game Logic variables
	Clock clock;
	Event event;
	View view;
	RenderWindow window;
	float deltaTime;
	bool hasStarted;
	bool acceptInput;
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
	bool** collidedPtrs;
	bool** startingBrickStatusPtrs;
	int currentGridSize;
	int GRID_NUMBERS[6];
	// Texts

	Text instructionsText;
	Font font;
	Text buttonText[16];
	//

	// SFX

	SoundBuffer brickCollisionBuffer;
	Sound brickCollisionSound;
	SoundBuffer paddleCollisionBuffer;
	Sound paddleCollisionSound;	
	SoundBuffer borderCollisionBuffer;
	Sound borderCollisionSound;
	SoundBuffer gameOverBuffer;
	Sound gameOverSound;
	SoundBuffer gameWinBuffer;
	Sound gameWinSound;
	Music BGMusic;

	//

	// Button
	RectangleShape button[16];
	Vector2f buttonSize;	
		
	//

	// Image Analyser
	int currentImageIndex;
	Image image[3];
	Texture displayTexture[3];
	Sprite displaySprite[3];
	Color** colorPtrs;
	Color minColorValue;
	Color maxColorValue;
	Color randomMinColor;
	Color randomMaxColor;
	bool isDisplaying;
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
	void changeDominateColor(int,int,int,int,int,int,int,int);
	void changeImage(int);
	void InitialiseDisplayImages();
	void RandomRGB();
	void ButtonHandling();
	//
};

