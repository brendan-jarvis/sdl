#include "bullet.h"
#include "constants.h"

Bullet::Bullet(float playerX, float playerY, float angle) {
  width = 1;
  height = 3;
  speed = 200;

  // Set the bullet's initial position
  // The bullet's position is the tip of the ship
  // Offset so we don't hit the player immediately
  centerX = playerX + 4.0 / 3.0 * height * cos(angle);
  centerY = playerY - 4.0 / 3.0 * height * sin(angle);

  // Set the bullet's initial velocity
  dX = speed * cos(angle);
  dY = -speed * sin(angle);

  // Set the bullet's initial state
  isAlive = true;
}

void Bullet::Update(float delta_time) {
  // Update the bullet's position
  centerX += dX * delta_time;
  centerY += dY * delta_time;

  // Check if the bullet is off the screen
  if (centerX < 0 || centerX > SCREEN_WIDTH || centerY < 0 ||
      centerY > SCREEN_HEIGHT) {
    isAlive = false;
  }

  // Update the bullet's drawing rectangle
  bulletDest.x = centerX - width / 2.0;
  bulletDest.y = centerY - height / 2.0;
  bulletDest.w = width;
  bulletDest.h = height;
}

void Bullet::Render(SDL_Renderer *renderer) {
  // Set the bullet's drawing color
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // Draw the bullet
  SDL_RenderFillRect(renderer, &bulletDest);
}

// Destructor
Bullet::~Bullet() {}
