#include <vector>
#include <SDL2/SDL.h>
#include "perlin_noise.h" 

class Background {
public:
  Background(SDL_Renderer* renderer, int width, int height);
  void drawStars(int numberOfStars);
  void drawPerlinStars();

private:
  SDL_Renderer* renderer;
  int width, height;
  std::vector<int> starColours;
  std::mt19937 gen;
  std::uniform_int_distribution<> dis;
  siv::PerlinNoise perlin;
  float frequency, threshold;

  int getStarColour(float noise);
};
