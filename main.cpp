#include <stdio.h>
#include <SDL2/SDL.h>

static int MOVEMENT_SPEED = 10;
static int LEFT_INNER_BORDER = 80;
static int RIGHT_INNER_BORDER = 560;
static int PL_WIDTH = 40;
static int PL_HEIGHT = 200;
static int BALL_x = 320;
static int BALL_Y = 240;
static int BALL_HEIGHT = 20;
static int BALL_WIDTH = 20;
static int DIRECTION_X = -4;
static int DIRECTION_Y = 0;

typedef struct
{
    int x;
    int y;
} Speed;

void move_rect(SDL_Surface *surface, SDL_Rect *player, Speed *speed)
{
    SDL_FillRect(surface, player, 0x00000000); // Clear previous position
    player->x += speed->x;
    player->y += speed->y;
    SDL_FillRect(surface, player, 0xffffffff); // Draw in new position
}

// For the movement of the ball we have (+, +) and (+, -) for the left side player
// For the movement of the ball we have (-, +) and (-, -) for the right side player

void move_ball(SDL_Surface *surface, SDL_Rect *ball, SDL_Rect *player1, SDL_Rect *player2, Speed *ball_speed, int &direction_x, int &direction_y)
{
    if (ball->x <= LEFT_INNER_BORDER)
    {
        if ((ball->y + ball->h < player1->y) || (ball->y > player1->y + player1->h))
        {
            SDL_FillRect(surface, ball, 0x00000000);
            ball->x = BALL_x;
            ball->y = BALL_Y;
        }

        if ((ball->y >= player1->y) && (ball->y <= player1->y + player1->h) && ball->x <= LEFT_INNER_BORDER)
        {
            if (direction_x == -4 && direction_y == 4)
            {
                direction_x = 4;
                ball_speed->x = direction_x;
            }
            if (direction_x == -4 && direction_y == -4)
            {
                direction_x = 4;
                ball_speed->x = direction_x;
            }
            if (direction_x == -4 && direction_y == 0)
            {
                direction_x = 4;
                direction_y = -4;
                ball_speed->x = direction_x;
                ball_speed->y = direction_y;
            }
        }
    }

    if (ball->x + ball->w >= RIGHT_INNER_BORDER)
    {
        if ((ball->y + ball->h < player2->y) || (ball->y > player2->y + player2->h))
        {
            SDL_FillRect(surface, ball, 0x00000000);
            ball->x = BALL_x;
            ball->y = BALL_Y;
        }

        if ((ball->y > player2->y) && (ball->y < player2->y + player2->h) && ball->x + ball->w >= RIGHT_INNER_BORDER)
        {
            if (direction_x == 4 && direction_y == 4)
            {
                direction_x = -4;
                ball_speed->x = direction_x;
            }
            if (direction_x == 4 && direction_y == -4)
            {
                direction_x = -4;
                ball_speed->x = direction_x;
            }
        }
    }

    if (ball->y <= 0)
    {
        if (direction_x == 4 && direction_y == -4)
        {
            direction_y = 4;
            ball_speed->y = direction_y;
        }
        if (direction_x == -4 && direction_y == -4)
        {
            direction_y = 4;
            ball_speed->y = direction_y;
        }
    }

    if (ball->y + ball->h >= surface->h)
    {
        if (direction_x == -4 && direction_y == 4)
        {
            direction_y = -4;
            ball_speed->y = direction_y;
        }
        if(direction_x == 4 && direction_y == 4)
        {
            direction_y = -4;
            ball_speed->y = direction_y;
        }
    }
    move_rect(surface, ball, ball_speed);
}

void move_player(SDL_Surface *surface, SDL_Rect *player, int direction)
{
    if (direction <= 0 && player->y <= 0)
        return;
    if (direction >= 0 && player->y + player->h == surface->h)
        return;
    Speed speed = (Speed){0, direction * MOVEMENT_SPEED};
    move_rect(surface, player, &speed);
}

int main(int argc, char *argv[])
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 948, 580, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect pl1 = (SDL_Rect){LEFT_INNER_BORDER - PL_WIDTH, 40, PL_WIDTH, PL_HEIGHT};
    SDL_Rect pl2 = (SDL_Rect){RIGHT_INNER_BORDER, 80, PL_WIDTH, PL_HEIGHT};
    SDL_Rect ball = (SDL_Rect){320, 240, 20, 20};
    SDL_Rect border = (SDL_Rect){330, 0, 1, 480};
    Uint32 color = 0xffffffff;

    // speed of the ball along x and y axis
    Speed ball_speed = (Speed){-4, 0};

    SDL_FillRect(surface, &pl1, color);
    SDL_FillRect(surface, &pl2, color);
    SDL_FillRect(surface, &ball, color);
    SDL_FillRect(surface, &border, color);
    SDL_UpdateWindowSurface(window);

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);

        if (keyState[SDL_SCANCODE_W])
        {
            move_player(surface, &pl1, -1);
        }
        if (keyState[SDL_SCANCODE_S])
        {
            move_player(surface, &pl1, +1);
        }
        if (keyState[SDL_SCANCODE_UP])
        {
            move_player(surface, &pl2, -1);
        }
        if (keyState[SDL_SCANCODE_DOWN])
        {
            move_player(surface, &pl2, +1);
        }

        // Move the ball
        move_ball(surface, &ball, &pl1, &pl2, &ball_speed, DIRECTION_X, DIRECTION_Y);

        // Redraw the border after moving the ball since it erases the border when it moves
        // along x direction
        SDL_FillRect(surface, &border, color);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(20);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
