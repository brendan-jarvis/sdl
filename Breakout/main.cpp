#include <iostream>
#include <SDL2/SDL.h>

// Structs/Classes
class Ball
{
public:
  float x, y, direction;
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
  srand(time(nullptr));

  // initialise the player
  player.x = (SCREEN_WIDTH / 2) - (player.width / 2); // middle of the screen
  player.y = SCREEN_HEIGHT - 30;                      // bottom of the screen
  player.width = 60;
  player.height = 5;
  player.acceleration = 0;
  player.speed = 10;
  player.is_alive = true;
  player.score = 0;
  player.lives = 3;

  // initialise the ball
  // starting from the player's paddle
  ball.x = player.x + (player.width / 2) - (ball.width / 2);
  ball.y = player.y - ball.height;
  ball.direction = (float)rand() / RAND_MAX * 2 * M_PI; // random direction - only up
  ball.width = 10;
  ball.height = 10;
  ball.is_alive = true;
  ball.speed = 100;
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
        player.x -= player.speed;
        break;
      case SDLK_RIGHT:
        player.x += player.speed;
        break;
      }
    case SDLK_r:
      setup();
      break;
    }
  }
}

void updateGame(void)
{
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  last_frame_time = SDL_GetTicks();

  // Update player position
  player.x += player.speed * player.acceleration * delta_time;

  // Update ball position
  ball.x += ball.speed * cos(ball.direction) * delta_time;
  ball.y += ball.speed * sin(ball.direction) * delta_time;

  // Check for player collision with the edge of the screen
  if (player.x <= 0)
  {
    player.x = 0;
    player.acceleration = 0;
  }
  else if (player.x >= SCREEN_WIDTH - player.width)
  {
    player.x = SCREEN_WIDTH - player.width;
    player.acceleration = 0;
  }

  // Check for ball collision with the edge of the screen
  if (ball.x <= 0)
  {
    ball.x = 0;
    ball.direction = M_PI - ball.direction;
  }
  else if (ball.x >= SCREEN_WIDTH - ball.width)
  {
    ball.x = SCREEN_WIDTH - ball.width;
    ball.direction = M_PI - ball.direction;
  }
  else if (ball.y <= 0)
  {
    ball.y = 0;
    ball.direction = -ball.direction;
  }
  else if (ball.y >= SCREEN_HEIGHT - ball.height)
  {
    // TODO: ball.is_alive = false;
    // TODO: player.lives--;
    ball.x = (SCREEN_WIDTH / 2) - (ball.width / 2);       // middle of the screen
    ball.y = (SCREEN_HEIGHT / 2) - (ball.height / 2);     // middle of the screen
    ball.direction = (float)rand() / RAND_MAX * 2 * M_PI; // random direction
  }

  // check for ball collision with the player
  // prevent the ball overlapping the player
  if (ball.y + ball.height >= player.y && ball.y <= player.y + player.height)
  {
    if (ball.x + ball.width >= player.x && ball.x <= player.x + player.width)
    {
      ball.y = player.y - ball.height;
      ball.direction = -ball.direction;
    }
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
