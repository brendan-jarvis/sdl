#include "star.h"
#include "constants.h"
#include <cstdlib>

std::vector<int> Star::getStarColours() {
  return {
      0xA3FFFF, // Blue-white star
      0xC6F6FF, // White-star
      0xFFFCB5, // Yellow-white star
      0xFFFA69, // Yellow star
      0xFFD100, // Golden star
      0xFF1700, // Red star
      0x810000, // Brown dwarf
  };
}

Star::Star() {
  std::vector<int> starColours = getStarColours();
  this->x = rand() % SCREEN_WIDTH;
  this->y = rand() % SCREEN_HEIGHT;
  this->colour = starColours[rand() % starColours.size()];
}
