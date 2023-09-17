#include <vector>

class Star {
public:
  float x, y;
  int colour;

  Star();

private:
  static std::vector<int> getStarColours();
};
