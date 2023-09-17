#include "player.h"
#include "constants.h"

Player::Player() {
    this->centerX = SCREEN_WIDTH / 2.0;
    this->centerY = SCREEN_HEIGHT / 2.0;
    this->size = 15;
    this->radius = this->size / 2.0;
    this->angle = 45 / 180.0 * M_PI; // convert to radians
    this->acceleration = 50;
    this->speed = 0;
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
    this->acceleration = 50;
    this->speed = 0;
    this->turnspeed = 360 / 180.0 * M_PI;
    this->isAlive = true;
    this->isAccelerating = false;
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
    // Update player position based on angle and acceleration
    // FIX: player is moving too fast when accelerating continuously
    centerX += speed * cos(angle) * delta_time;
    centerY -= speed * sin(angle) * delta_time;

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
      speed += acceleration * delta_time;
    } else {
      speed *= pow(friction, delta_time);
    }
}

