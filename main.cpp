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

void setup(void)
{
  // initialise the star background

  // initialise the player
  player.x = 20;
  player.y = 20;
  player.width = 20;
  player.height = 20;
  player.velX = 0;
  player.velY = 0;
  player.speed = 5;
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

void processInput(void) {}

void updateGame(void) {}

void renderOutput(void) {}

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

  return 0;
}
