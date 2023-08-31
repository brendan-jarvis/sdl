#include <stdlib.h>
#include <SDL2/SDL.h>

int main(void)
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(480, 320, 0, &window, &renderer);

  bool quit = false;

  // Event handler
  SDL_Event e;

  // Color variables
  int r = 255;
  int g = 0;
  int b = 0;

  // While application is running
  while (!quit)
  {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) // poll for event
    {
      // User requests quit
      if (e.type == SDL_QUIT) // unless player manually quits
      {
        quit = true;
      }
    }

    // Delay to reduce CPU usage
    SDL_Delay(10);

    // Check for key press
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
    {
      quit = true;
    }

    // Reset color to red when user presses 'r'
    if (state[SDL_SCANCODE_R])
    {
      r = 255;
      g = 0;
      b = 0;

      SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }

    // Reset color to green when user presses 'g'
    if (state[SDL_SCANCODE_G])
    {
      r = 0;
      g = 255;
      b = 0;

      SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }

    // Reset color to blue when user presses 'b'
    if (state[SDL_SCANCODE_B])
    {
      r = 0;
      g = 0;
      b = 255;

      SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return (0);
}
