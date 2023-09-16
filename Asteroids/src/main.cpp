#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <iostream>

#include "SDL2/SDL_render.h"
#include "constants.h"

// NOTE: Structs/Classes
class Player {
public:
  float centerX, centerY, acceleration, speed, turnspeed, angle, friction;
  int lives, score, radius, size;
  SDL_Point linePoints[4];
  bool isAlive, isAccelerating;

  Player() {
    this->centerX = SCREEN_WIDTH / 2.0;
    this->centerY = SCREEN_HEIGHT / 2.0;
    this->size = 30;
    this->radius = this->size / 2.0;
    this->angle = 45 / 180.0 * M_PI; // convert to radians
    this->acceleration = 10;
    this->speed = 0;
    this->turnspeed = 0.25;
    this->isAlive = true;
    this->isAccelerating = false;
    this->score = 0;
    this->lives = 3;
    this->friction = 0.7;
  }

  void Reset(void) {
    this->centerX = SCREEN_WIDTH / 2.0;
    this->centerY = SCREEN_HEIGHT / 2.0;
    this->size = 30;
    this->radius = this->size / 2.0;
    this->angle = 45 / 180.0 * M_PI; // convert to radians
    this->acceleration = 10;
    this->speed = 0;
    this->turnspeed = 0.25;
    this->isAlive = true;
    this->isAccelerating = false;
    this->score = 0;
    this->lives = 3;
    this->friction = 0.7;
  }

  void Accelerate(void) { isAccelerating = true; }

  void StopAccelerating(void) { isAccelerating = false; }

  void RotateLeft(void) {
    angle += turnspeed;
  } // HACK: doesn't use rotation, directly mutates angle instead

  void RotateRight(void) { angle -= turnspeed; }

  void Update(float delta_time) {
    // Update player position based on angle and acceleration
    centerX += speed * cos(angle) * delta_time;
    centerY -= speed * sin(angle) * delta_time;

    // Update player drawing points
    // linePoints[0] is the nose of the ship
    linePoints[0].x = centerX + 4.0 / 3.0 * radius * cos(angle);
    linePoints[0].y = centerY - 4.0 / 3.0 * radius * sin(angle);
    // linePoints[1] is the tip of the left wing
    linePoints[1].x = centerX - radius * (2.0 / 3.0 * cos(angle) + sin(angle));
    linePoints[1].y = centerY + radius * (2.0 / 3.0 * sin(angle) - cos(angle));
    // linePoints[2] is the tip of the right wing
    linePoints[2].x = centerX - radius * (2.0 / 3.0 * cos(angle) - sin(angle));
    linePoints[2].y = centerY + radius * (2.0 / 3.0 * sin(angle) + cos(angle));
    // linePoints[3] is the nose of the ship
    linePoints[3].x = linePoints[0].x;
    linePoints[3].y = linePoints[0].y;

    // Check if accelerating or not
    // Update the speed
    if (isAccelerating) {
      // FIX: isAccelerating is always false
      speed += acceleration * delta_time;
    } else {
      speed *= friction;
    }
  }
};

class Star {
public:
  float x, y;
  int brightness;
};

// NOTE: Globals
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player;
Star stars[100];
SDL_Event event;
TTF_Font *font = nullptr;

void setup(void) {
  // Player player = Player();

  // TODO: initialise the asteroids
}

bool initialiseWindow(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }
  window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED,
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
        player.Reset();
        break;
      case SDLK_UP:
        player.Accelerate();
        break;
      case SDLK_LEFT:
        player.RotateLeft();
        break;
      case SDLK_RIGHT:
        player.RotateRight();
        break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        player.StopAccelerating();
        break;
      }
      break;
    }
  }
}

void updateGame(void) {
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0;

  last_frame_time = SDL_GetTicks();

  // Update player
  player.Update(delta_time);

  // Update star brightness
  // FIX: make stars twinkle
  //  float time = SDL_GetTicks() / 1000.0f;
  //  for (int i = 0; i < 100; i++) {
  //    stars[i].brightness = 100 + (sin(time + stars[i].brightness) + 1) * 25;
  //  }

  // TODO: Check for collision with window bounds
  if (player.centerX + player.radius >= SCREEN_WIDTH) {
    player.centerX = SCREEN_WIDTH - player.radius;
  } else if (player.centerX - player.radius <= 0) {
    player.centerX = 0 + player.radius;
  }
  if (player.centerY + player.radius >= SCREEN_HEIGHT) {
    player.centerY = SCREEN_HEIGHT - player.radius;
  } else if (player.centerY - player.radius <= 0) {
    player.centerY = 0 + player.radius;
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

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLines(renderer, player.linePoints, 4);

  // Red dot in center of player
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawPoint(renderer, player.centerX, player.centerY);

  // Create surface to contain text
  SDL_Color color = {255, 255, 255};
  std::string score =
      "Score: " + std::to_string(player.score) +
      "    Lives: " + std::to_string(player.lives) +
      "    Speed: " + std::to_string(player.speed) +
      "    Accelerating: " + std::to_string(player.isAccelerating);

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
