class Ball
{
public:
  float x, y, direction;
  int width, height, lives, score, speed, acceleration;
  bool is_alive;
};

class Player
{
public:
  float x, y;
  int width, height, lives, score, speed, acceleration;
  bool is_alive;
};

class Brick
{
public:
  float x, y;
  int width, height;
  bool is_alive;
};

