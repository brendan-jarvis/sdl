#include "input.h"
#include "constants.h"
#include "player.h"
#include <SDL2/SDL.h>

void processInput(bool &gameIsRunning, Player *player, SDL_Event &event) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      gameIsRunning = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        gameIsRunning = false;
        break;
      case SDLK_r:
        player->Reset();
        break;
      }
      break;
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        player->StopAccelerating();
        break;
      case SDLK_LEFT:
        player->StopRotating();
        break;
      case SDLK_RIGHT:
        player->StopRotating();
        break;
      }
      break;
    }
  }

  if (state[SDL_SCANCODE_UP]) {
    player->Accelerate();
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->RotateLeft();
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->RotateRight();
  }
}
