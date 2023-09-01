/* main.cpp
 * The entry point for the program.
 */

#include <iostream>
// #include <stdlib.h>
#include <SDL2/SDL.h>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Globals
int game_is_running = false;
int last_frame_time = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool initialiseWindow()
{
}

void processInput() {}

void updateGame() {}

void renderOutput() {}

int main(void)
{
  game_is_running = initialiseWindow();

  while (game_is_running)
  {
    processInput();
    updateGame();
    renderOutput();
  }

  return 0;
}
