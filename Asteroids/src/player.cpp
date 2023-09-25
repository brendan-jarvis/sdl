#include "player.h"
#include "SDL2/SDL_render.h"
#include "SDL2_image/SDL_image.h"
#include "constants.h"
#include <iostream>
#include <vector>

Player::Player(SDL_Renderer *renderer) {
  this->centerX = SCREEN_WIDTH / 2.0;
  this->centerY = SCREEN_HEIGHT / 2.0;
  this->size = 15;
  this->radius = this->size / 2.0;
  this->angle = 90 / 180.0 * M_PI; // convert to radians
  this->acceleration = 50;
  this->turnspeed = 360 / 180.0 * M_PI;
  this->isAlive = true;
  this->isAccelerating = false;
  this->score = 0;
  this->lives = 3;
  this->friction = 0.7;
  this->hasExploded = false;

  // Load textures
  this->explosionTexture =
      IMG_LoadTexture(renderer, "../assets/sprites/orange_explosion.png");

  if (!explosionTexture) {
    std::cout << "Failed to load explosion texture: " << SDL_GetError()
              << std::endl;
  }

  this->boosterTexture =
      IMG_LoadTexture(renderer, "../assets/sprites/booster.png");

  if (!boosterTexture) {
    std::cout << "Failed to load booster texture: " << SDL_GetError()
              << std::endl;
  }

  // Create the SDL_Rects for each explosion frame
  for (int i = 0; i < 4; i++) {
    SDL_Rect frame;
    frame.x = i * 8;
    frame.y = 0;
    frame.w = 8;
    frame.h = 8;
    explosionFrames.push_back(frame);
  }

  // Create the SDL_Rects for each booster frame
  for (int i = 0; i < 4; i++) {
    SDL_Rect frame;
    frame.x = i * 8;
    frame.y = 0;
    frame.w = 4;
    frame.h = 5;
    boosterFrames.push_back(frame);
  }

  // Create the SDL_Rect for the destination of the explosion sprite
  explosionRect = {100, 100, size * 2, size * 2};

  // Create the SDL_Rect for the destination of the booster sprite
  boosterRect = {100, 100, size * 2, size * 2};
}

void Player::Reset(void) {
  this->centerX = SCREEN_WIDTH / 2.0;
  this->centerY = SCREEN_HEIGHT / 2.0;
  this->size = 15;
  this->radius = this->size / 2.0;
  this->angle = 90 / 180.0 * M_PI; // convert to radians
  this->acceleration = 100;
  this->turnspeed = 360 / 180.0 * M_PI;
  this->isAlive = true;
  this->isAccelerating = false;
  this->acceleration_X = 0;
  this->acceleration_Y = 0;
  this->lives -= 1;
  this->friction = 0.7;
  this->hasExploded = false;
  this->explosionFrame = 0;
}

void Player::Accelerate(void) { isAccelerating = true; }

void Player::StopAccelerating(void) { isAccelerating = false; }

void Player::RotateLeft(void) { rotation = turnspeed; }

void Player::RotateRight(void) { rotation = -turnspeed; }

void Player::StopRotating(void) { rotation = 0; }

void Player::Update(float delta_time) {
  // Update player angle with rotation
  angle += rotation * delta_time;

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
    acceleration_X += acceleration * cos(angle) * delta_time;
    acceleration_Y -= acceleration * sin(angle) * delta_time;
  } else {
    // apply friction
    acceleration_X *= pow(friction, delta_time);
    acceleration_Y *= pow(friction, delta_time);
  }

  // Update the position
  centerX += acceleration_X * delta_time;
  centerY += acceleration_Y * delta_time;

  // Wrap around the screen
  if (centerX < 0 - radius) {
    centerX = SCREEN_WIDTH + radius;
  } else if (centerX > SCREEN_WIDTH + radius) {
    centerX = 0 - radius;
  }
  if (centerY < 0 - radius) {
    centerY = SCREEN_HEIGHT + radius;
  } else if (centerY > SCREEN_HEIGHT + radius) {
    centerY = 0 - radius;
  }

  // Update the explosion destination
  explosionDest.x = centerX - size;
  explosionDest.y = centerY - size;
  explosionDest.w = size * 2;
  explosionDest.h = size * 2;

  // Update the booster destination
  boosterAngle = -angle * 180.0 / M_PI + 90; // convert to degrees
  float offset_X = 6.0 / 3.0 * radius * cos(angle - M_PI);
  float offset_Y = -6.0 / 3.0 * radius * sin(angle - M_PI);

  boosterDest.x = centerX + offset_X - radius;
  boosterDest.y = centerY + offset_Y - radius;
  boosterDest.w = radius * 2;
  boosterDest.h = radius * 2;
}

void Player::Render(SDL_Renderer *renderer) {
  // Draw the player
  if (isAlive) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLines(renderer, linePoints, 4);
  }

  if (!isAlive) {
    // Stop player movement
    this->acceleration_X = 0;
    this->acceleration_Y = 0;

    // Render the explosion
    int numberOfFrames = explosionFrames.size();
    int frameDelay = 500;
    int currentFrame = SDL_GetTicks() / frameDelay % numberOfFrames;

    // Draw small explosion as glowing wreckage 
    if (hasExploded) {
      SDL_RenderCopy(renderer, explosionTexture, &explosionFrames[0],
                     &explosionDest);
    } else {
      SDL_RenderCopy(renderer, explosionTexture, &explosionFrames[currentFrame],
                     &explosionDest);

      if (currentFrame == numberOfFrames - 1) {
        hasExploded = true;
      }
    }
  }

  // If isAccelerating is true, draw the thruster
  if (isAccelerating) {
    int numberOfBoosterFrames = boosterFrames.size();
    int boosterFrameDelay = 100;
    int currentBoosterFrameIndex =
        SDL_GetTicks() / boosterFrameDelay % numberOfBoosterFrames;

    // Render the booster texture
    SDL_RenderCopyEx(renderer, boosterTexture,
                     &boosterFrames[currentBoosterFrameIndex], &boosterDest,
                     boosterAngle, NULL, SDL_FLIP_NONE);
  }
}

Player::~Player() {
  SDL_DestroyTexture(boosterTexture);
  SDL_DestroyTexture(explosionTexture);
}
