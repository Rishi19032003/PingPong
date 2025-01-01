#include <stdio.h>
#include <SDL2/SDL.h>

static int MOVEMENT_SPEED = 10;

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
    SDL_Window *window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 648, 480, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect pl1 = (SDL_Rect){40, 40, 40, 200};
    SDL_Rect pl2 = (SDL_Rect){560, 80, 40, 200};
    SDL_Rect ball = (SDL_Rect){320, 240, 20, 20};
    SDL_Rect border = (SDL_Rect){330, 0, 1, 480};
    Uint32 color = 0xffffffff;

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

        SDL_UpdateWindowSurface(window);
        SDL_Delay(20);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
