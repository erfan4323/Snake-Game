#include <deque>
#include <iostream>
#include "raylib.h"
#include "raymath.h"

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
	double currTime = GetTime();

	if (currTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currTime;
		return true;
	}
	return false;
}

bool ElementInDequeue(Vector2 element, std::deque<Vector2> dequeue) 
{
	for (const auto& i : dequeue) 
	{
		if (Vector2Equals(i, element))
			return true;
	}
	return false;
}

class Food
{
private:

public:
	Vector2 position;
	Texture2D texture;

	Food(std::deque<Vector2> snakeBody)
	{
		Image image = LoadImage("Resources/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody)
	{
		Vector2 position = GenerateRandomCell();

		do {
			position = GenerateRandomCell();
			std::cout << "food position : " << position.x << " " << position.y << std::endl;
		} while (ElementInDequeue(position, snakeBody) || position.x >= cellCount || position.y >= cellCount);

		return position;
	}
};

class Snake
{
private:

public:
	std::deque<Vector2> body = { Vector2{ 6, 9 }, Vector2{ 5, 9 }, Vector2{ 4, 9 } };
	Vector2 dir = { 1, 0 };
	bool addsegment = false;

	Snake() {}
	~Snake() {}

	void Draw()
	{
		Rectangle segment;
		for (const auto& i : body)
		{
			segment = Rectangle{ (float)i.x * cellSize, (float)i.y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 10, darkGreen);
		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], dir));

		if (addsegment)
			addsegment = false;

		else
			body.pop_back();
	}

	void Reset()
	{
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		dir = { 1, 0 };
	}
};

class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;

	Game(){}
	~Game(){}

	void Draw()
	{
		BeginDrawing();
		{
			ClearBackground(green);
			food.Draw();
			snake.Draw();
		}
		EndDrawing();
	}

	void Update()
	{
		if (running)
		{
			snake.Update();
			CheckCollisionsWithFood();
			CheckColisionsWithEdges();
			CheckColisionsWithBody();
		}
	}

	void CheckCollisionsWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
			snake.addsegment = true;
		}
	}

	void CheckColisionsWithEdges()
	{
		if (snake.body[0].x == cellCount || snake.body[0].x == -1)
			GameOver();

		if (snake.body[0].y == cellCount || snake.body[0].y == -1)
			GameOver();
	}

	void CheckColisionsWithBody()
	{
		std::deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();

		if (ElementInDequeue(snake.body[0], headlessBody))
			GameOver();
	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
	}
};

int main()
{
	int resolution = cellCount * cellSize;
	InitWindow(resolution, resolution, "Retro Snake");
	Image icon = LoadImage("Resources/LogoSbake.ico");
	SetWindowIcon(icon);

	SetTargetFPS(60);
	
	Game game = Game();

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_UP) && game.snake.dir.y != 1) 
		{
			game.snake.dir = { 0, -1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_DOWN) && game.snake.dir.y != -1) 
		{
			game.snake.dir = { 0, 1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_LEFT) && game.snake.dir.x != 1)
		{
			game.snake.dir = { -1, 0 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.dir.x != -1) 
		{
			game.snake.dir = { 1, 0 };
			game.running = true;
		}

		if (EventTriggered(0.2)) game.Update();

		game.Draw();
	}

	CloseAudioDevice();
	return 0;
}