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
  angle = rand() % 360;

  // speed is random between 50 and 100
  speed = rand() % 50 + 50;
  
  // isAlive is true
  isAlive = true;
}

void Asteroid::Update(float deltaTime){
  // Move the asteroid
  centerX += cos(angle * M_PI / 180.0) * speed * deltaTime;
  centerY += sin(angle * M_PI / 180.0) * speed * deltaTime;

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
