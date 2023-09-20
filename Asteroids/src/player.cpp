#include "player.h"
#include "SDL2/SDL_render.h"
#include "SDL2_image/SDL_image.h"
#include "constants.h"
#include <iostream> // TODO: Remove this
#include <vector>

Player::Player() {
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
  this->currentBoosterFrameIndex = 0;
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
  this->score = 0;
  this->lives = 3;
  this->friction = 0.7;
  this->currentBoosterFrameIndex = 0;
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
}

void Player::Render(SDL_Renderer *renderer) {

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawLines(renderer, linePoints, 4);

  // If player.isAccelerating is true, draw the thruster
  // if (isAccelerating) {
  // Load the booster.png sprite sheet
  SDL_Texture *boosterTexture =
      IMG_LoadTexture(renderer, "../assets/sprites/booster.png");

  // Create a vector of SDL_Rects to hold the sprite sheet frames
  std::vector<SDL_Rect> boosterFrames;
  // Create the SDL_Rects for each frame
  SDL_Rect frame[4];
  for (int i = 0; i < 4; i++) {
    // Each booster is 4 px wide and 5 px tall
    // There are 4 frames in the sprite sheet
    // These are separated by a 4 px gap
    frame[i].x = i * 8;
    frame[i].y = 0;
    frame[i].w = 4;
    frame[i].h = 5;
    boosterFrames.push_back(frame[i]);
  }

  // Create a SDL_Rect for the destination of the sprite
  SDL_Rect dest;
  // TODO:
  // The destination is fixed below the ship
  // It should be rotated correctly with the ship
  dest.x = centerX - radius;
  dest.y = centerY + radius;
  dest.w = radius * 2;
  dest.h = radius * 2;

  // Calculate the angle for the booster based on the ship's angle
  double boosterAngle = -angle * 180.0 / M_PI + 90;

  // Render the booster texture
  SDL_RenderCopyEx(renderer, boosterTexture,
                   &boosterFrames[currentBoosterFrameIndex], &dest,
                   boosterAngle, NULL, SDL_FLIP_NONE);

  // Don't forget to free the boosterTexture when you're done with it
  SDL_DestroyTexture(boosterTexture);
  //}
}

void Player::Animate() {
  // Constants for animation
  const int NUM_BOOSTER_FRAMES = 4;

  if (isAccelerating) {
    currentBoosterFrameIndex += 1; // Switch to the next frame
    if (currentBoosterFrameIndex >= NUM_BOOSTER_FRAMES) {
      currentBoosterFrameIndex = 0; // Go back to the first frame
    }
  } else {
    currentBoosterFrameIndex = 0; // Reset to the first frame
  }
}
