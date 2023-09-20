#pragma once
#include <SDL2/SDL.h>

class Player {
public:
  float centerX, centerY, acceleration, acceleration_X, acceleration_Y,
      turnspeed, angle, friction, rotation;
  int lives, score, radius, size, currentBoosterFrameIndex;
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
  void Animate();
  void Render(SDL_Renderer *renderer);
};
