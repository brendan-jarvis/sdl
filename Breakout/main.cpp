#include <iostream>
#include <SDL2/SDL.h>

// Structs/Classes
class Ball
{
public:
  float x, y;
  int width, height, lives, score, speed, acceleration;
  bool is_alive;
};

class Player
{
public:
  float x, y;
  int width, height, lives, score, speed, acceleration;
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
Ball ball;
SDL_Event event;

void setup(void)
{
  // initialise the player
  player.x = (SCREEN_WIDTH / 2) - (player.width / 2); // middle of the screen
  player.y = SCREEN_HEIGHT - 30;                      // bottom of the screen
  player.width = 30;
  player.height = 5;
  player.acceleration = 0;
  player.speed = 100;
  player.is_alive = true;
  player.score = 0;
  player.lives = 3;

  // initialise the ball
  ball.x = (SCREEN_WIDTH / 2) - (ball.width / 2);   // middle of the screen
  ball.y = (SCREEN_HEIGHT / 2) - (ball.height / 2); // middle of the screen
  ball.width = 5;
  ball.height = 5;
  ball.is_alive = true;
  ball.speed = 3;
  ball.acceleration = 0;
}

bool initialiseWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }
  window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
        player.acceleration = -player.speed;
        break;
      case SDLK_RIGHT:
        player.acceleration = player.speed;
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
  player.x += player.acceleration * delta_time;

  // TODO: add friction to player acceleration

  // Check for player collision with the edge of the screen
  if (player.x <= 0)
  {
    player.x = 0;
  }
  else if (player.x >= SCREEN_WIDTH - player.width)
  {
    player.x = SCREEN_WIDTH - player.width;
  }
}

void renderOutput(void)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect playerRect = {(int)player.x, (int)player.y, player.width, player.height};
  SDL_Rect ballRect = {(int)ball.x, (int)ball.y, ball.width, ball.height};

  SDL_RenderFillRect(renderer, &playerRect);
  SDL_RenderFillRect(renderer, &ballRect);

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
