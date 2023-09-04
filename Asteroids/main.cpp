#include <iostream>
#include <SDL2/SDL.h>

// Structs/Classes
class Player
{
public:
  float x, y, rotation, acceleration, speed, turnspeed;
  int width, height, lives, score;
  SDL_Point linePoints[4];
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
  player.width = 5;
  player.height = 10;
  player.rotation = 0;
  player.acceleration = 0;
  player.speed = 3;
  player.turnspeed = 1;
  player.is_alive = true;
  player.score = 0;
  player.lives = 3;
  player.linePoints[0] = {(int)player.x - player.width, (int)player.y};
  player.linePoints[1] = {(int)player.x, (int)player.y - player.height};
  player.linePoints[2] = {(int)player.x + player.width, (int)player.y};
  player.linePoints[3] = {(int)player.x - player.width, (int)player.y};

  // initialise the asteroids
}

bool initialiseWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
  }
  window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
      case SDLK_UP:
        player.acceleration += player.speed;
        break;
      case SDLK_DOWN:
        player.acceleration -= player.speed;
        break;
      case SDLK_LEFT:
        player.rotation -= player.turnspeed;
        break;
      case SDLK_RIGHT:
        player.rotation += player.turnspeed;
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
  player.y += player.acceleration * delta_time;

  // TODO: Update player rotation

  // Update player line points
  player.linePoints[0] = {(int)player.x - player.width, (int)player.y};
  player.linePoints[1] = {(int)player.x, (int)player.y - player.height};
  player.linePoints[2] = {(int)player.x + player.width, (int)player.y};
  player.linePoints[3] = {(int)player.x - player.width, (int)player.y};

  // TODO: add friction to player acceleration

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
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLines(renderer, player.linePoints, 4);
  // SDL_Point points[4] = {{310, 240}, {320, 220}, {330, 240}, {310, 240}};
  // SDL_RenderDrawLines(renderer, points, 4);

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
