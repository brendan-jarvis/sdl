#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "SDL2/SDL_render.h"
#include "SDL2_image/SDL_image.h"
#include "constants.h"
#include <iostream>
#include "bullet.h"

class Player {
public:
  float centerX, centerY, acceleration, acceleration_X, acceleration_Y,
      turnspeed, angle, friction, rotation, boosterAngle, firingSpeed, firingTimer;
  int lives, score, radius, size;
  SDL_Point linePoints[4];
  bool isAlive, isAccelerating, isFiring, hasExploded;
  SDL_Texture *explosionTexture, *boosterTexture;
  SDL_Rect explosionRect, boosterRect, explosionDest, boosterDest;
  std::vector<SDL_Rect> explosionFrames, boosterFrames;
  std::vector<Bullet *> bullets; // Vector of pointers to Bullet objects

  Player(SDL_Renderer *renderer);
  ~Player();
  void Reset(void);
  void Accelerate(void);
  void StopAccelerating(void);
  void RotateLeft(void);
  void RotateRight(void);
  void StopRotating(void);
  void Fire(void);
  void StopFiring(void);
  void Update(float current_time, SDL_Renderer *renderer);
  void Render(SDL_Renderer *renderer);
};
