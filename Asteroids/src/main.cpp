#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "SDL2/SDL_render.h"
#include "asteroid.h"
#include "constants.h"
#include "player.h"
#include "star.h"

// NOTE: Globals
int gameIsRunning = false;
int lastFrameTime = 0;
int frameCount = 0;
float timeCount = 0.0f;
float frameRate = 0.0f;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player;
Star stars[100];
std::vector<Asteroid> asteroids;
SDL_Texture *backgroundTexture = NULL;
SDL_Event event;
TTF_Font *font = nullptr;

void setup(void) {
  // Seed random number generator
  srand(time(NULL));

  // PERF: Create stars surface
  // Doing this here saves us having to draw the stars every frame
  SDL_Surface *backgroundSurface = NULL;
  backgroundSurface =
      SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

  // Create a temporary SDL_Renderer to draw on the surface
  SDL_Renderer *tempRenderer = SDL_CreateSoftwareRenderer(backgroundSurface);

  // Draw stars onto the surface
  for (int i = 0; i < 100; i++) {
    SDL_SetRenderDrawColor(tempRenderer, stars[i].colour >> 16,
                           (stars[i].colour >> 8) & 0xFF,
                           stars[i].colour & 0xFF, 255);
    SDL_RenderDrawPoint(tempRenderer, stars[i].x, stars[i].y);
  }

  // Create texture from surface pixels
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

  // Free the surface and the temporary renderer as we no longer need them
  SDL_FreeSurface(backgroundSurface);
  SDL_DestroyRenderer(tempRenderer);

  // Push 10 asteroids to the vector
  for (int i = 0; i < 10; i++) {
    asteroids.push_back(Asteroid(player.centerX, player.centerY));
  }
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

  return true;
}

void processInput(void) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      gameIsRunning = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        gameIsRunning = false;
        break;
      case SDLK_r:
        player.Reset();
        break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        player.StopAccelerating();
        break;
      case SDLK_LEFT:
        player.StopRotating();
        break;
      case SDLK_RIGHT:
        player.StopRotating();
        break;
      }
      break;
    }
  }

  if (state[SDL_SCANCODE_UP]) {
    player.Accelerate();
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player.RotateLeft();
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player.RotateRight();
  }
}

void calculateFramerate(float deltaTime) {
  frameCount++;
  timeCount += deltaTime;

  if (timeCount >= 1.0f) {              // If more than one second has passed
    frameRate = frameCount / timeCount; // Calculate the frame rate
    timeCount -= 1.0f;                  // Subtract one second from time count
    frameCount = 0;                     // Reset frame count
  }
}

void updateGame(void) {
  float currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - lastFrameTime) / 1000.0;

  lastFrameTime = currentTime;

  // Update player
  player.Update(deltaTime);

  // Update asteroids
  for (int i = 0; i < 10; i++) {
    if (asteroids[i].isAlive) {
      asteroids[i].Update(deltaTime);
    }
  }

  // Calculate frame rate
  calculateFramerate(deltaTime);
}

void renderOutput(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw background texture
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // Draw the asteroids
  SDL_Texture *asteroidTexture =
      IMG_LoadTexture(renderer, "../assets/noun-asteroid-5303658.svg");

  if (!asteroidTexture) {
    std::cout << "Failed to load texture: " << IMG_GetError() << std::endl;
  }

  for (int i = 0; i < 10; i++) {
    if (asteroids[i].isAlive) {
      SDL_Rect asteroidRect;
      asteroidRect.x = asteroids[i].centerX;
      asteroidRect.y = asteroids[i].centerY;
      asteroidRect.w = asteroids[i].size;
      asteroidRect.h = asteroids[i].size;

      SDL_RenderCopyEx(renderer, asteroidTexture, NULL, &asteroidRect,
                       asteroids[i].rotation, NULL, SDL_FLIP_NONE);
    }
  }

  // Draw the player
  player.Render(renderer);

  // Create surface to contain text
  SDL_Color color = {255, 255, 255};
  std::string score = "Score: " + std::to_string(player.score) +
                      "    Lives: " + std::to_string(player.lives) +
                      "    FPS: " + std::to_string((int)frameRate);

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

int limitFPS(void) {
  int timeSinceLastFrame = SDL_GetTicks() - lastFrameTime;
  if (timeSinceLastFrame < frameDelay) {
    return frameDelay - timeSinceLastFrame;
  }
  return 0;
}

int main(void) {
  gameIsRunning = initialiseWindow();

  setup();

  while (gameIsRunning) {
    processInput();
    updateGame();
    renderOutput();

    if (limitFPS() > 0) {
      SDL_Delay(limitFPS());
    }
  }

  destroyWindow();

  return 0;
}
