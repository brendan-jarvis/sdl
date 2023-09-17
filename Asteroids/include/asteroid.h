class Asteroid {
public:
  float centerX, centerY, angle, speed;
  int size;
  bool isAlive;

  Asteroid(float playerX, float playerY);
  void Update(float deltaTime);
};
