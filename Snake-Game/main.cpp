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

class Food
{
private:

public:
	Vector2 position;
	Texture2D texture;

	Food()
	{
		Image image = LoadImage("Resources/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomPos()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
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

/*class Game
{
public:
	Game(){}
	~Game(){}

	

	void Draw()
	{
		//BeginDrawing();
		//{
			//ClearBackground(green);
			food.Draw();
			snake.Draw();
		//}
		//EndDrawing();
	}

	void Update()
	{
		snake.Update();
	}
};*/

int main()
{
	int resolution = cellCount * cellCount;
	InitWindow(resolution, resolution, "Retro Snake");

	SetTargetFPS(60);
	
	Snake snake = Snake();
	Food food = Food();

	while (!WindowShouldClose())
	{
		if (EventTriggered(0.4)) snake.Update();
		
		if (IsKeyPressed(KEY_UP) && snake.dir.y != 1) snake.dir = { 0, -1 };
		if (IsKeyPressed(KEY_DOWN) && snake.dir.y != -1) snake.dir = { 0, 1 };
		if (IsKeyPressed(KEY_LEFT) && snake.dir.x != 1) snake.dir = { -1, 0 };
		if (IsKeyPressed(KEY_RIGHT) && snake.dir.x != -1) snake.dir = { 1, 0 };


		BeginDrawing();
		{
			ClearBackground(green);
			snake.Draw();
			food.Draw();
		}
		EndDrawing();

	}

	CloseAudioDevice();
	return 0;
}