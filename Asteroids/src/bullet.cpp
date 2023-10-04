#include "bullet.h"
#include "SDL2/SDL_render.h"
#include "constants.h"

Bullet::Bullet(float playerX, float playerY, float angle) {
  width = 1;
  height = 3;
  speed = 200;
  this->angle = angle * (180.0 / M_PI) - 90.0; // convert to degrees

  // Set the bullet's initial position
  // The bullet's position is the tip of the ship
  // Offset so we don't hit the player
  centerX = playerX + 4.0 / 3.0 * height * cos(angle);
  centerY = playerY - 4.0 / 3.0 * height * sin(angle);

  // Set the bullet's initial velocity
  dX = speed * cos(angle);
  dY = -speed * sin(angle);

  // Set the bullet's initial state
  isAlive = true;

  // Set the bullet's destination rectangle
  bulletSrc.x = 0;
  bulletSrc.y = 0;
  bulletSrc.w = width;
  bulletSrc.h = height;
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

  // Update the bullet's destination rectangle
  bulletDest.x = centerX - width / 2.0;
  bulletDest.y = centerY - height / 2.0;
  bulletDest.w = width;
  bulletDest.h = height;
}

// FIX: Bullet texture is not rendering
// Bullets are currently invisible
// Only the 'bounding box' is visible
void Bullet::Render(SDL_Renderer *renderer, SDL_Texture *bulletTexture) {
  this->bulletTexture = bulletTexture;

  std::cout << "Bullet::Render" << std::endl;
  std::cout << "bulletTexture: " << &bulletTexture << std::endl;
  std::cout << "bulletSrc: " << bulletSrc.x << ", " << bulletSrc.y << ", "
            << bulletSrc.w << ", " << bulletSrc.h << std::endl;
  std::cout << "bulletDest: " << bulletDest.x << ", " << bulletDest.y << ", "
            << bulletDest.w << ", " << bulletDest.h << std::endl;

  SDL_RenderCopyEx(renderer, bulletTexture, NULL, &bulletSrc, this->angle, NULL,
                   SDL_FLIP_NONE);

  // Draw a bounding box around the bullet
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderDrawRect(renderer, &bulletDest);
}

// Destructor
Bullet::~Bullet() {}
