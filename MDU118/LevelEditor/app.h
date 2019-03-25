#pragma once
#include "SFML/Graphics.hpp"
using namespace sf;

const int NUM_OF_BRICKS_ROWS = 5;
const int NUM_OF_BRICK_COLUMNS = 5;
class App
{
	//Game Logic variables
	Clock clock;
	Event event;
	View view;
	RenderWindow window;
	float deltaTime;
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
	Vector2f paddleSize;
	
	//

	// Bricks

	RectangleShape bricks[NUM_OF_BRICKS_ROWS][NUM_OF_BRICK_COLUMNS];
	Vector2f sizeOfBricks;
	bool collided[NUM_OF_BRICKS_ROWS][NUM_OF_BRICK_COLUMNS];

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
	
	//
};

