#include "player.h"
#include "SDL2/SDL_render.h"
#include "constants.h"
#include <vector>

Player::Player() {
  this->centerX = SCREEN_WIDTH / 2.0;
  this->centerY = SCREEN_HEIGHT / 2.0;
  this->size = 15;
  this->radius = this->size / 2.0;
  this->angle = 45 / 180.0 * M_PI; // convert to radians
  this->acceleration = 50;
  this->turnspeed = 360 / 180.0 * M_PI;
  this->isAlive = true;
  this->isAccelerating = false;
  this->score = 0;
  this->lives = 3;
  this->friction = 0.7;
}

void Player::Reset(void) {
  this->centerX = SCREEN_WIDTH / 2.0;
  this->centerY = SCREEN_HEIGHT / 2.0;
  this->size = 15;
  this->radius = this->size / 2.0;
  this->angle = 45 / 180.0 * M_PI; // convert to radians
  this->acceleration = 100;
  this->turnspeed = 360 / 180.0 * M_PI;
  this->isAlive = true;
  this->isAccelerating = false;
  this->acceleration_X = 0;
  this->acceleration_Y = 0;
  this->score = 0;
  this->lives = 3;
  this->friction = 0.7;
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
  if (isAccelerating) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Point thrusterPoints[4];
    thrusterPoints[0].x =
        centerX - radius * (2.0 / 3.0 * cos(angle) + sin(angle));
    thrusterPoints[0].y =
        centerY + radius * (2.0 / 3.0 * sin(angle) - cos(angle));
    thrusterPoints[1].x =
        centerX - radius * (2.0 / 3.0 * cos(angle) - sin(angle));
    thrusterPoints[1].y =
        centerY + radius * (2.0 / 3.0 * sin(angle) + cos(angle));
    thrusterPoints[2].x = centerX - radius * (5.0 / 3.0 * cos(angle));
    thrusterPoints[2].y = centerY + radius * (5.0 / 3.0 * sin(angle));
    thrusterPoints[3].x =
        centerX - radius * (2.0 / 3.0 * cos(angle) + sin(angle));
    thrusterPoints[3].y =
        centerY + radius * (2.0 / 3.0 * sin(angle) - cos(angle));
    SDL_RenderDrawLines(renderer, thrusterPoints, 4);

    /*TODO: optimise this code to be usable
    * SDL_Point leftPoint, rightPoint, bottomPoint;
    * leftPoint.x = centerX - radius * (2.0 / 3.0 * cos(angle) + sin(angle));
    *  leftPoint.y = centerY + radius * (2.0 / 3.0 * sin(angle) - cos(angle));
    *  rightPoint.x = centerX - radius * (2.0 / 3.0 * cos(angle) - sin(angle));
    *  rightPoint.y = centerY + radius * (2.0 / 3.0 * sin(angle) + cos(angle));
    *  bottomPoint.x = centerX - radius * (5.0 / 3.0 * cos(angle));
    *  bottomPoint.y = centerY + radius * (5.0 / 3.0 * sin(angle));
    *  const std::vector<SDL_Vertex> verts = {
    *      {
    *          SDL_FPoint{static_cast<float>(leftPoint.x),
    *                     static_cast<float>(leftPoint.y)},
    *          SDL_Color{255, 255, 255, 255},
    *          SDL_FPoint{0.0},
    *      },
    *      {
    *          SDL_FPoint{static_cast<float>(rightPoint.x),
    *                     static_cast<float>(rightPoint.y)},
    *          SDL_Color{255, 165, 0, 255},
    *          SDL_FPoint{0.0},
    *      },
    *      {
    *          SDL_FPoint{static_cast<float>(bottomPoint.x),
    *                     static_cast<float>(bottomPoint.y)},
    *          SDL_Color{255, 255, 255, 255},
    *          SDL_FPoint{0.0},
    *      },
    *  };
    *
    *  SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr,
    *                     0);
    *  SDL_RenderPresent(renderer);
    */
  }
}
