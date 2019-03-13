#pragma once
#include "SFML/Graphics.hpp"
using namespace sf;

int const N = 2;
class App
{
	Clock clock;
	Event event;
	View view;
	RenderWindow window;
	CircleShape circle[N];
	RectangleShape paddle;
	float deltaTime;

	Texture backgroundTexture;
	Sprite backgroundSprite;

	Texture ballTexture;
	Sprite ballSprite;

	Texture paddleTexture;
	

	float radius;
	
	int sign1;
	int sign2;
	Vector2f speed;
	float xSpeed[N];
	float ySpeed[N];


	float paddleVelocity;
	Vector2f paddleSize;

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
	void CircleMovement();
	void PaddleMovement();
	

};

