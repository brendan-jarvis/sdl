class Asteroid {
public:
  float centerX, centerY, dx, dy, rotation, rotationSpeed;
  int size;
  bool isAlive;

  Asteroid(float playerX, float playerY);
  void Update(float deltaTime);
  void Animate();
};
