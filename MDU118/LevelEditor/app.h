#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
using namespace sf;

const int NUM_OF_BRICKS_ROWS = 10;
const int NUM_OF_BRICK_COLUMNS = 10;
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

	RectangleShape bricks[NUM_OF_BRICKS_ROWS][NUM_OF_BRICK_COLUMNS];
	RectangleShape brickShadows[NUM_OF_BRICKS_ROWS][NUM_OF_BRICK_COLUMNS];
	Vector2f sizeOfBricks;
	bool collided[NUM_OF_BRICKS_ROWS][NUM_OF_BRICK_COLUMNS];
	Texture brickTexture;

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
	RectangleShape button;
	Vector2f buttonSize;	
		
	//

	// Image Analyser
	Image image;
	Color color[NUM_OF_BRICKS_ROWS][NUM_OF_BRICK_COLUMNS];

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
	//
};

