#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <iostream>

#include "SDL2/SDL_render.h"
#include "constants.h"

// Structs/Classes
class Player {
public:
  float centerX, centerY, acceleration, speed, turnspeed, angle;
  int lives, score, radius, size;
  SDL_Point linePoints[4];
  bool is_alive, isAccelerating;

  void Accelerate(void) { isAccelerating = true; }

  void Decelerate(void) { isAccelerating = false; }

  void RotateLeft(void) { angle -= turnspeed; }

  void RotateRight(void) { angle += turnspeed; }

  void Update(float delta_time) {
    centerX += acceleration * cos(angle) * delta_time;
    centerY += acceleration * sin(angle) * delta_time;

    // Update player drawing points
    // TODO: update x and y values based on z-axis rotation
    // angle = rotation * M_PI / 180.0;
    // linePoints[0] is the nose of the ship
    linePoints[0].x = centerX + radius * cos(angle);
    linePoints[0].y = centerY - radius * sin(angle);
    // linePoints[1] is the tip of the left wing
    linePoints[1].x = centerX - radius * (cos(angle) + sin(angle));
    linePoints[1].y = centerY + radius * (sin(angle) - cos(angle));
    // linePoints[2] is the tip of the right wing
    linePoints[2].x = centerX - radius * (cos(angle) - sin(angle));
    linePoints[2].y = centerY + radius * (sin(angle) + cos(angle));
    // linePoints[3] is the nose of the ship
    linePoints[3].x = linePoints[0].x;
    linePoints[3].y = linePoints[0].y;

    if (isAccelerating) {
      acceleration += speed;
    } else {
      acceleration *= 0.997;
    }
  }
};

class Star {
public:
  float x, y;
  int brightness;
};

// Globals
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player;
Star stars[100];
SDL_Event event;
TTF_Font *font = nullptr;

void setup(void) {
  // initialise the player
  player.centerX = SCREEN_WIDTH / 2.0;
  player.centerY = SCREEN_HEIGHT / 2.0;
  player.size = 30;
  player.radius = player.size / 2.0;
  player.angle = 90 / 180.0 * M_PI; // convert to radians
  player.acceleration = 0;
  player.speed = 0.5;
  player.turnspeed = 1;
  player.is_alive = true;
  player.score = 0;
  player.lives = 3;

  // TODO: initialise the asteroids
}

bool initialiseWindow(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }
  window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    std::cout << "SDL could not create window! SDL_Error: " << SDL_GetError()
              << std::endl;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    std::cout << "SDL could not create renderer! SDL_Error: " << SDL_GetError()
              << std::endl;
  }

  if (TTF_Init() == -1) {
    std::cout << "Could not initailize SDL2_ttf, error: " << TTF_GetError()
              << std::endl;
  } else {
    std::cout << "SDL2_ttf system ready to go!" << std::endl;
  }

  font = TTF_OpenFont("assets/fonts/Asteroids.ttf", 32);

  if (font == nullptr) {
    std::cout << "Could not load font" << std::endl;
    return false;
  }

  // Initialise stars - runs only once
  for (int i = 0; i < 100; i++) {
    stars[i].x = rand() % SCREEN_WIDTH;
    stars[i].y = rand() % SCREEN_HEIGHT;
    stars[i].brightness =
        rand() % 50 + 100; // Sets brightness to random value from 100 to 150
  }

  return true;
}

void processInput(void) {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        game_is_running = false;
        break;
      case SDLK_r:
        setup();
        break;
      case SDLK_UP:
        player.Accelerate();
        break;
      case SDLK_DOWN:
        player.Decelerate();
        break;
      case SDLK_LEFT:
        player.RotateLeft();
        break;
      case SDLK_RIGHT:
        player.RotateRight();
        break;
      }
    }
  }
}

void updateGame(void) {
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  last_frame_time = SDL_GetTicks();

  // Update player
  player.Update(delta_time);

  // Update star brightness
  // TODO: make stars twinkle

  //  float time = SDL_GetTicks() / 1000.0f;
  //  for (int i = 0; i < 100; i++) {
  //    stars[i].brightness = 100 + (sin(time + stars[i].brightness) + 1) * 25;
  //  }

  // Check for collision with window bounds
  if (player.centerX - player.size / 2.0 <= 0) {
    player.centerX = 0 - player.size / 2.0;
  }
  if (player.centerX + player.size / 2.0 >= SCREEN_WIDTH - player.size) {
    player.centerX = SCREEN_WIDTH - player.size / 2.0;
  }
  if (player.centerY - player.size / 2.0 <= 0) {
    player.centerY = 0 - player.size / 2.0;
  }
  if (player.centerY >= SCREEN_HEIGHT - player.size) {
    player.centerY = SCREEN_HEIGHT - player.size / 2.0;
  }
}

void renderOutput(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw stars
  for (int i = 0; i < 100; i++) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, stars[i].brightness);
    SDL_RenderDrawPoint(renderer, stars[i].x, stars[i].y);
  }

  // TODO: triangular player ship
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLines(renderer, player.linePoints, 4);

  // Create surface to contain text
  SDL_Color color = {255, 255, 255};
  std::string score = "Score: " + std::to_string(player.score) +
                      "    Lives: " + std::to_string(player.lives) +
                      "    Rotation: " + std::to_string(player.angle) +
                      "    Angle: " + std::to_string(player.angle);

  // Check if font is valid
  if (font != nullptr) {
    SDL_Surface *text = TTF_RenderText_Solid(font, score.c_str(), color);
    if (!text) {
      std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
    } else {
      // Create texture
      SDL_Texture *text_texture;

      text_texture = SDL_CreateTextureFromSurface(renderer, text);

      // Get the width and height of the texture
      int text_width = text->w;
      int text_height = text->h;

      // Setup the destination rectangle to be at the position we want
      SDL_Rect dest = {10, 5, text_width, text_height};

      SDL_RenderCopy(renderer, text_texture, NULL, &dest);

      // Free the surface and texture
      SDL_FreeSurface(text);
      SDL_DestroyTexture(text_texture);
    }
  }

  SDL_RenderPresent(renderer);
}

void destroyWindow(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(void) {
  game_is_running = initialiseWindow();

  setup();

  while (game_is_running) {
    processInput();
    updateGame();
    renderOutput();
  }

  destroyWindow();

  return 0;
}
