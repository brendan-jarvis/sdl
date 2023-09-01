/* main.cpp
 * The entry point for the program.
 */

#include <iostream>
// #include <stdlib.h>
#include <SDL2/SDL.h>

// Structs/Classes
class Player
{
public:
  int x, y, width, height, velX, velY, speed, lives, score;
  bool is_alive;
};

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Globals
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player;
SDL_Event event;

void setup(void)
{
  // initialise the star background

  // initialise the player
  player.x = (SCREEN_WIDTH / 2) - (player.width / 2);
  player.y = (SCREEN_HEIGHT / 2) - (player.height / 2);
  player.width = 20;
  player.height = 20;
  player.velX = 0;
  player.velY = 0;
  player.speed = 1;
  player.is_alive = true;
  player.score = 0;
  player.lives = 3;

  // initialise the asteroids
}

bool initialiseWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }
  window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window)
  {
    std::cout << "SDL could not create window! SDL_Error: " << SDL_GetError() << std::endl;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer)
  {
    std::cout << "SDL could not create renderer! SDL_Error: " << SDL_GetError() << std::endl;
  }

  return true;
}

void processInput(void)
{
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        game_is_running = false;
        break;
      case SDLK_LEFT:
        player.velX = -player.speed;
        break;
      case SDLK_a:
        player.velX = -player.speed;
        break;
      case SDLK_RIGHT:
        player.velX = player.speed;
        break;
      case SDLK_d:
        player.velX = player.speed;
        break;
      case SDLK_UP:
        player.velY = -player.speed;
        break;
      case SDLK_w:
        player.velY = -player.speed;
        break;
      case SDLK_DOWN:
        player.velY = player.speed;
        break;
      case SDLK_s:
        player.velY = player.speed;
        break;
      }
    }
  }
}

void updateGame(void)
{
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  last_frame_time = SDL_GetTicks();

  // Update player position
  player.x += player.velX * delta_time;
  player.y += player.velY * delta_time;

  // Check for collision with window bounds
  if (player.x <= 0)
  {
    player.x = 0;
  }
  if (player.x >= SCREEN_WIDTH - player.width)
  {
    player.x = SCREEN_WIDTH - player.width;
  }
  if (player.y <= 0)
  {
    player.y = 0;
  }
  if (player.y >= SCREEN_HEIGHT - player.height)
  {
    player.y = SCREEN_HEIGHT - player.height;
  }
}

void renderOutput(void)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // TODO: triangular player ship
  SDL_Rect playerRect = {player.x, player.y, player.width, player.height};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &playerRect);

  SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(void)
{
  game_is_running = initialiseWindow();

  setup();

  while (game_is_running)
  {
    processInput();
    updateGame();
    renderOutput();
  }

  destroyWindow();

  return 0;
}
