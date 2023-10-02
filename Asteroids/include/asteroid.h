#pragma once
#include "SDL2/SDL_render.h"
class Asteroid {
public:
  float centerX, centerY, dx, dy, rotation, rotationSpeed;
  int size, radius;
  bool isAlive;
  SDL_Texture *asteroidTexture;
  SDL_Rect asteroidRect;

  Asteroid(SDL_Renderer *renderer, float playerX, float playerY);
  void Update(float deltaTime);
  void Render(SDL_Renderer *renderer);
  bool CheckPlayerCollision(float playerX, float playerY);
  bool CheckAsteroidCollision(Asteroid *asteroid);
};

