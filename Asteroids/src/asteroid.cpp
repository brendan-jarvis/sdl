#include "asteroid.h"
#include "SDL2/SDL_render.h"
#include "SDL2_image/SDL_image.h"
#include "constants.h"
#include <cmath>
#include <iostream>

Asteroid::Asteroid(SDL_Renderer *renderer, float playerX, float playerY) {
  // size is 32
  size = 32;

  // radius is half of size
  radius = size / 2.0;

  // Generate random position
  centerX = rand() % (SCREEN_WIDTH - size) + size / 2.0;
  centerY = rand() % (SCREEN_HEIGHT - size) + size / 2.0;

  // if within 100 pixels of player, try again
  while (centerX > playerX - 100 && centerX < playerX + 100) {
    centerX = rand() % (SCREEN_WIDTH - size) + size / 2.0;
  }
  while (centerY > playerY - 100 && centerY < playerY + 100) {
    centerY = rand() % (SCREEN_HEIGHT - size) + size / 2.0;
  }

  // angle is random between 0 and 360
  float angle = rand() % 360;

  // speed is random between 25 and 30
  float speed = rand() % 5 + 25;

  // Calculate dx dy for the asteroid
  // This simplifies updating the position
  dx = speed * cos(angle);
  dy = speed * sin(angle);

  // rotation is random between 0 and 360
  // rotationSpeed is random between -0.5 and 0.5
  rotation = rand() % 360;
  rotationSpeed = rand() % 100 / 100.0 - 0.5;

  // Create asteroidRect
  asteroidRect = {static_cast<int>(centerX - radius),
                  static_cast<int>(centerY - radius), size, size};

  // PERF: Can load this once and pass it to the constructor
  // Load the asteroid texture
  asteroidTexture = IMG_LoadTexture(renderer, "../assets/sprites/asteroid.png");

  if (!asteroidTexture) {
    std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
  }

  // isAlive is true
  isAlive = true;
}

void Asteroid::Update(float deltaTime) {
  centerX += dx * deltaTime;
  centerY += dy * deltaTime;

  // Wrap around the screen
  if (centerX < 0 - size / 2.0) {
    centerX = SCREEN_WIDTH + size / 2.0;
  } else if (centerX > SCREEN_WIDTH + size / 2.0) {
    centerX = 0 - size / 2.0;
  }
  if (centerY < 0 - size / 2.0) {
    centerY = SCREEN_HEIGHT + size / 2.0;
  } else if (centerY > SCREEN_HEIGHT + size / 2.0) {
    centerY = 0 - size / 2.0;
  }

  // Animation updates
  rotation += this->rotationSpeed;

  // Update asteroidRect
  asteroidRect = {static_cast<int>(centerX - radius),
                  static_cast<int>(centerY - radius), size, size};
}

void Asteroid::Render(SDL_Renderer *renderer) {
  SDL_RenderCopyEx(renderer, asteroidTexture, NULL, &asteroidRect, rotation,
                   NULL, SDL_FLIP_NONE);
}

bool Asteroid::CheckPlayerCollision(float playerX, float playerY) {
  // Checks collisions with a simple radius check
  if (playerX > centerX - radius && playerX < centerX + radius &&
      playerY > centerY - radius && playerY < centerY + radius) {
    return true;
  } else {
    return false;
  }
}

bool Asteroid::CheckAsteroidCollision(Asteroid *asteroid) {
  // Checks collisions with a simple radius check
  if (asteroid->centerX > centerX - radius &&
      asteroid->centerX < centerX + radius &&
      asteroid->centerY > centerY - radius &&
      asteroid->centerY < centerY + radius) {
    return true;
  } else {
    return false;
  }
}
