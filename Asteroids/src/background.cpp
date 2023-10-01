#include "background.h"

Background::Background(SDL_Renderer *renderer, int width, int height)
    : renderer(renderer), width(width), height(height),
      starColours({
          0xA3FFFF, // #A3FFFF Blue-white star
          0xC6F6FF, // #C6F6FF White-star
          0xFFFCB5, // #FFFCB5 Yellow-white star
          0xFFFA69, // #FFFA69 Yellow star
          0xFFD100, // #FFD100 Golden star
          0xFF1700, // #FF1700 Red star
          0x810000, // #810000 Brown dwarf
      }),
      gen(std::random_device()()), dis(0, starColours.size() - 1),
      perlin(gen()), frequency(0.01), threshold(0.1) {}

std::vector<std::string> Background::getBackgroundImages() {
  std::vector<std::string> pngFiles;
  std::string path = "../assets/backgrounds";
  
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(path)) {
    if (entry.path().extension() == ".png") {
      pngFiles.push_back(entry.path().string());
    }
  }
  
  return pngFiles;
}

int Background::getStarColour(float noise) {
  int index = (int)(noise * starColours.size());
  return starColours[index];
}

void Background::drawStars(int numberOfStars) {
  std::uniform_int_distribution<> disX(0, width - 1);
  std::uniform_int_distribution<> disY(0, height - 1);
  std::uniform_int_distribution<> disColour(0, starColours.size() - 1);

  for (int i = 0; i < numberOfStars; ++i) {
    int x = disX(gen);
    int y = disY(gen);
    int colour = starColours[disColour(gen)];

    SDL_SetRenderDrawColor(renderer, colour >> 16, (colour >> 8) & 0xFF,
                           colour & 0xFF, 255);
    SDL_RenderDrawPoint(renderer, x, y);
  }
}

/**
 * This works but doesn't generate very nice looking stars as background.
 * Better to use an image or random pixel stars.
 */
void Background::drawPerlinStars() {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      double noise = perlin.noise2D(x * frequency, y * frequency);
      if (noise > threshold) {
        int colour = getStarColour(noise);
        int alpha = (int)(noise * 255);
        SDL_SetRenderDrawColor(renderer, colour >> 16, (colour >> 8) & 0xFF,
                               colour & 0xFF, alpha);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }
}
