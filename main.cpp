#include <stdlib.h>
#include <SDL2/SDL.h>

int main(void)
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(480, 320, 0, &window, &renderer);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  bool quit = false;

  // Event handler
  SDL_Event e;

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
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return (0);
}