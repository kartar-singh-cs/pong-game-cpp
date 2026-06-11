#include <iostream>
#include <raylib.h>
using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Yellow = Color{243, 213, 91, 255};
Color Light_Green = Color{129, 204, 154, 255};

int player_score = 0;
int cpu_score = 0;

class Ball
{

public:
    float x;
    float y;
    int radius;
    int speed_x;
    int speed_y;

    // Constructor
    Ball(int x, int y, int r, int speed_x, int speed_y)
    {
        this->x = x;
        this->y = y;
        radius = r;
        this->speed_x = speed_x;
        this->speed_y = speed_y;
    }

    void draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update()
    {
        x = x + speed_x;
        y = y + speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) // Collision from Up and Down
        {
            speed_y = speed_y * -1;
        }

        // Collision from Left and Right Side
        if (x + radius >= GetScreenWidth())
        {
            cpu_score++;
            speed_x = speed_x * -1;
        }
        if (x - radius <= 0)
        {
            player_score++;
            speed_x = speed_x * -1;
        }
    }
};

class Paddle // Paddle Class for Paddles
{
protected:
    // For Keeping Limit of Screen
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }

        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    int x;
    int y;
    int width;
    int height;
    int speed_y;

    Paddle(int x, int y, int width, int height, int speed_y) // Constructor For Paddle
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->speed_y = speed_y;
    }

    void draw() // Drawing Paddle
    {
       DrawRectangleRounded(Rectangle{(float)x, (float)y, (float)width, (float)height}, 0.8, 0, WHITE);; // Right Side Paddle
    }

    void Update() // Updating Paddle
    {

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            y = y - speed_y;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            y = y + speed_y;
        }

        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{

public:
    CpuPaddle(int x, int y, int width, int height, int speed_y) : Paddle(x, y, width, height, speed_y) {};

    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed_y;
        }

        if (y + height / 2 <= ball_y)
        {
            y = y + speed_y;
        }

        LimitMovement();
    }
};

int main()
{

    int screen_width = 1280;
    int screen_height = 800;

    Ball ball((screen_width) / 2, (screen_height) / 2, 20, 7, 7); // Ball Class

    Paddle p1(screen_width - 35, screen_height / 2 - 60, 25, 120, 5);

    CpuPaddle cpu(10, screen_height / 2 - 60, 25, 120, 5);

    InitWindow(screen_width, screen_height, "Pong By Kartar Singh");
    SetTargetFPS(60);

    while (WindowShouldClose() == false)
    {

        BeginDrawing();

        // UPDATING
        ball.Update();
        p1.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)p1.x, (float)p1.y, (float)p1.width, (float)p1.height}))
        {
            ball.speed_x = ball.speed_x * -1;
            ball.x = p1.x - ball.radius; // push ball out
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{(float)cpu.x, (float)cpu.y, (float)cpu.width, (float)cpu.height}))
        {
            ball.speed_x = ball.speed_x * -1;
            ball.x = cpu.x + cpu.width + ball.radius; // push ball out
        }

        ClearBackground(Dark_Green);

        // DRAWING
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green); // Player Side Rectangle Color
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);          // Center Circle Color
        ball.draw();                                                                // Drawing Ball
        cpu.draw();                                                                 // Drawing Left Computer Paddle
        p1.draw();                                                                  // Drawing Right Player Paddle

        DrawText(TextFormat("%i", cpu_score), screen_width / 4, 25, 80, WHITE); // Score For CPU

        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4, 25, 80, WHITE); // Score For Player

        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE); // Center Line

        EndDrawing();
    }

    CloseWindow();

    return 0;
}