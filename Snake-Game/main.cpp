#include <deque>
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

		while (ElementInDequeue(position, snakeBody))
			position = GenerateRandomCell();

		return position;
	}
};

class Snake
{
private:

public:
	std::deque<Vector2> body = { Vector2{ 6, 9 }, Vector2{ 5, 9 }, Vector2{ 4, 9 } };
	Vector2 dir = { 1, 0 };

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
		body.pop_back();
		body.push_front(Vector2Add(body[0], dir));
	}
};

class Game
{
public:
	Game(){}
	~Game(){}

	Snake snake = Snake();
	Food food = Food(snake.body);

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
		snake.Update();
		CheckCollisionWithFood();
	}

	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
		}
	}
};

int main()
{
	int resolution = cellCount * cellCount;
	InitWindow(resolution, resolution, "Retro Snake");

	SetTargetFPS(60);
	
	Game game = Game();

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_UP) && game.snake.dir.y != 1) game.snake.dir = { 0, -1 };
		if (IsKeyPressed(KEY_DOWN) && game.snake.dir.y != -1) game.snake.dir = { 0, 1 };
		if (IsKeyPressed(KEY_LEFT) && game.snake.dir.x != 1) game.snake.dir = { -1, 0 };
		if (IsKeyPressed(KEY_RIGHT) && game.snake.dir.x != -1) game.snake.dir = { 1, 0 };

		if (EventTriggered(0.4)) game.Update();

		game.Draw();
	}

	CloseAudioDevice();
	return 0;
}