#pragma once
#include <SDL2/SDL.h>

// NOTE: Structs/Classes
class Player {
public:
  float centerX, centerY, acceleration, speed, turnspeed, angle, friction,
      rotation;
  int lives, score, radius, size;
  SDL_Point linePoints[4];
  bool isAlive, isAccelerating;

  Player();
  void Reset(void);
  void Accelerate(void);
  void StopAccelerating(void);
  void RotateLeft(void);
  void RotateRight(void);
  void StopRotating(void);
  void Update(float delta_time);
};

