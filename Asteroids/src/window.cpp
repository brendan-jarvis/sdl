#include "window.h"
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "constants.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

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

