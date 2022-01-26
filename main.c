#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>
#include "game.h"

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("error init sdl: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow(
      "curdle",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      640,
      640,
      0
  );

  if (window == NULL) {
    printf("error init sdl window: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  if (renderer == NULL) {
    printf("error init sdl renderer: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Delay(3000);

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
