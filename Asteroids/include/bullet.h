#include "SDL2_image/SDL_image.h"
#include "asteroid.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class Bullet {
public:
  float centerX, centerY, dX, dY, angle;
  int width, height, speed;
  bool isAlive;
  SDL_Texture *bulletTexture;
  SDL_Rect bulletSrc, bulletDest;

  Bullet(float playerX, float playerY, float playerAngle);
  void Update(float deltaTime);
  void Render(SDL_Renderer *renderer, SDL_Texture *bulletTexture);
  bool CheckAsteroidCollision(Asteroid *asteroid);
  bool CheckPlayerCollision(float playerX, float playerY);
  ~Bullet();
};
