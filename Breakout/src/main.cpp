#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#include "constants.h"

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

class Brick
{
public:
  float x, y;
  int width, height;
  bool is_alive;
};

// Globals
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player;
Ball ball;
Brick bricks[24];
SDL_Event event;
TTF_Font *font;

int getRandomNumber(int min, int max)
{
  return rand() % (max - min + 1) + min;
}

void drawBricks(Brick bricks[]) 
{
  int minX = 50;
  int minY = SCREEN_HEIGHT / 4.0;
  for (int i = 0; i < 24; i++)
  {
    bricks[i].x = minX + (i % 8) * 70;
    bricks[i].y = minY + (i / 8.0) * 30;
    bricks[i].width = 50;
    bricks[i].height = 20;
    bricks[i].is_alive = true;
  }
}



void setup(void)
{
  // initialise the player
  player.x =
      (SCREEN_WIDTH / 2.0) - (player.width / 2.0); // middle of the screen
  player.y = SCREEN_HEIGHT - 30;                   // bottom of the screen
  player.width = 60;
  player.height = 5;
  player.acceleration = 0;
  player.speed = 100;
  player.is_alive = true;
  player.score = 0;
  player.lives = 3;

  // initialise the ball
  ball.x = SCREEN_WIDTH / 2.0;
  ball.y = SCREEN_WIDTH / 2.0;
  ball.direction = (rand() % 360) * (M_PI / 180);
  ball.width = 10;
  ball.height = 10;
  ball.is_alive = true;
  ball.speed = 0;
  ball.acceleration = 0;

  // initialise bricks on the screen
  drawBricks(bricks);
}

bool initialiseWindow(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }
  window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window)
  {
    std::cout << "SDL could not create window! SDL_Error: " << SDL_GetError()
              << std::endl;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer)
  {
    std::cout << "SDL could not create renderer! SDL_Error: " << SDL_GetError()
              << std::endl;
  }

  if (TTF_Init() == -1)
  {
    std::cout << "Could not initailize SDL2_ttf, error: " << TTF_GetError()
              << std::endl;
  }
  else
  {
    std::cout << "SDL2_ttf system ready to go!" << std::endl;
  }

  font = TTF_OpenFont("assets/fonts/Asteroids.ttf", 32);

  if (font == nullptr)
  {
    std::cout << "Could not load font" << std::endl;
    return false;
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
      case SDLK_w: // Increase ball speed
        ball.speed += 10;
        break;
      case SDLK_UP: // Increase player speed
        player.speed += 10;
        break;
      case SDLK_DOWN: // Decrease player speed
        player.speed -= 10;
        break;
      case SDLK_s: // Decrease ball speed
        ball.speed -= 10;
        break;
      case SDLK_r: // Reset the game
        setup();
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

  // speed up the ball after setup
  if (ball.speed < 100)
  {
    ball.speed += 1;
  }

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
    player.lives--;
    ball.x = (SCREEN_WIDTH / 2.0) - (ball.width / 2.0); // middle of the screen
    ball.y =
        (SCREEN_HEIGHT / 2.0) - (ball.height / 2.0);      // middle of the screen
    ball.direction = (float)rand() / RAND_MAX * 2 * M_PI; // random direction
    ball.speed = 100;
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

  // check for ball collision with the bricks
  for (int i = 0; i < 24; i++)
  {
    if (bricks[i].is_alive)
    {
      if (ball.y + ball.height >= bricks[i].y &&
          ball.y <= bricks[i].y + bricks[i].height)
      {
        if (ball.x + ball.width >= bricks[i].x &&
            ball.x <= bricks[i].x + bricks[i].width)
        {
          bricks[i].is_alive = false;
          ball.direction = -ball.direction;
          player.score++;
        }
      }
    }
  }
}

void renderOutput(void)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect playerRect = {(int)player.x, (int)player.y, player.width,
                         player.height};
  SDL_Rect ballRect = {(int)ball.x, (int)ball.y, ball.width, ball.height};

  // draw for the bricks
  for (int i = 0; i < 24; i++)
  {
    if (bricks[i].is_alive)
    {
      SDL_Rect brickRect = {(int)bricks[i].x, (int)bricks[i].y, bricks[i].width,
                            bricks[i].height};
      SDL_RenderFillRect(renderer, &brickRect);
    }
  }

  // Create surface to contain text
  SDL_Color color = {255, 255, 255};
  std::string score = "Score: " + std::to_string(player.score) +
                      "    Lives: " + std::to_string(player.lives);

  // Check if font is valid
  if (font != nullptr)
  {
    SDL_Surface *text = TTF_RenderText_Solid(font, score.c_str(), color);
    if (!text)
    {
      std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
    }
    else
    {
      // Create texture
      SDL_Texture *text_texture;

      text_texture = SDL_CreateTextureFromSurface(renderer, text);

      // Get the width and height of the texture
      int text_width = text->w;
      int text_height = text->h;

      // Setup the destination rectangle to be at the position we want
      SDL_Rect dest = {0, 0, text_width, text_height};

      SDL_RenderCopy(renderer, text_texture, NULL, &dest);

      // Free the surface and texture
      SDL_FreeSurface(text);
      SDL_DestroyTexture(text_texture);
    }
  }

  SDL_RenderFillRect(renderer, &playerRect);
  SDL_RenderFillRect(renderer, &ballRect);

  SDL_RenderPresent(renderer);
}

void destroyWindow(void)
{
  TTF_CloseFont(font);
  TTF_Quit();
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
