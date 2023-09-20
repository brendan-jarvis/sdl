#include "asteroid.h"
#include "constants.h"
#include <cmath>
#include <iostream>

Asteroid::Asteroid(float playerX, float playerY) {
  // size is random between 25 and 30
  size = rand() % 5 + 25;

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
}

void Asteroid::Animate() { rotation += rotationSpeed; }
