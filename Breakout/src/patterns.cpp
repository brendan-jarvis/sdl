#include <cmath>
#include <iostream>

#include "classes.h"

void archimedesSpiral(int n, int r, int a, int b, int *xArray, int *yArray) {
  for (int i = 0; i < n; i++) {
    xArray[i] = r * cos(i) + a;
    yArray[i] = r * sin(i) + b;
  }
}

void archimedesSpiralBricks(Brick *bricks, int n, int brickWidth,
                            int brickHeight, int centerX, int centerY,
                            int offset, int spacing, int numArms) {
  // Adjust the angle increment to create arms
  double angleIncrement = 2.0 * M_PI / ((float)n / numArms);

  for (int arm = 0; arm < numArms; arm++) {
    double currentAngle = arm * (2.0 * M_PI / numArms);

    for (int i = 0; i < n / numArms; i++) {
      // Calculate the radial distance from the center
      double r = offset + spacing * currentAngle;

      // Calculate the x and y coordinates for the brick's center
      double x = r * cos(currentAngle) + centerX;
      double y = r * sin(currentAngle) + centerY;

      // Update the brick's position (adjust for center-to-corner positioning)
      bricks[arm * (n / numArms) + i].x = x - brickWidth / 2.0;
      bricks[arm * (n / numArms) + i].y = y - brickHeight / 2.0;

      bricks[arm * (n / numArms) + i].width = brickWidth;
      bricks[arm * (n / numArms) + i].height = brickHeight;
      bricks[arm * (n / numArms) + i].is_alive = true;
      
      // check for bricks with x or y values outside the screen
      // if so, set is_alive to false otherwise set it to true
      int SCREEN_WIDTH = centerX * 2;
      int SCREEN_HEIGHT = centerY * 2;
    
      if (bricks[arm * (n / numArms) + i].x < 0 || bricks[arm * (n / numArms) + i].x > SCREEN_WIDTH - brickWidth) {
        bricks[arm * (n / numArms) + i].is_alive = false;
      } else if (bricks[arm * (n / numArms) + i].y < 0 || bricks[arm * (n / numArms) + i].y > SCREEN_HEIGHT - brickHeight || bricks[arm * (n / numArms) + i].y <= 20) {
        // if the brick is outside the screen or near the top of the screen, set is_alive to false
        bricks[arm * (n / numArms) + i].is_alive = false;
      } else {
        bricks[arm * (n / numArms) + i].is_alive = true;
      }

      // Update the angle for the next brick
      currentAngle += angleIncrement;
    }
  }
}
