#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Player {
public:
  float centerX, centerY, acceleration, acceleration_X, acceleration_Y,
      turnspeed, angle, friction, rotation, boosterAngle;
  int lives, score, radius, size;
  SDL_Point linePoints[4];
  bool isAlive, isAccelerating, hasExploded;
  SDL_Texture *explosionTexture, *boosterTexture;
  SDL_Rect explosionRect, boosterRect, explosionDest, boosterDest;
  std::vector<SDL_Rect> explosionFrames, boosterFrames;

  Player(SDL_Renderer *renderer);
  ~Player();
  void Reset(void);
  void Accelerate(void);
  void StopAccelerating(void);
  void RotateLeft(void);
  void RotateRight(void);
  void StopRotating(void);
  void Update(float current_time);
  void Render(SDL_Renderer *renderer);
};
