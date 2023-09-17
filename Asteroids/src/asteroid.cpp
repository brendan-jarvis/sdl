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
