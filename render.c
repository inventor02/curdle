#include "render.h"
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#include <limits.h>

int start_window() {

  // Create our window pointer
  SDL_Window *window;
  SDL_Renderer *renderer;

  // Attempts to initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO != 0)) {
    printf("Error initialising SDL: %s\n", SDL_GetError());
    return CURDLE_SDL_INITILISATION_FAILURE;
  }

  TTF_Init();

  // Create a window centered on the screen with set width and height
  window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 240 * CURDLE_WINDOW_SCALE, 135 * CURDLE_WINDOW_SCALE, 0);

  if (window == NULL) {
    printf("Error creating window: %s\n", SDL_GetError());
    return CURDLE_SDL_WINDOW_FAILURE;

  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    printf("Error creating renderer: %s\n", SDL_GetError());
    return CURDLE_SDL_RENDERER_FAILURE;
  }

  // Set background colour
  SDL_SetRenderDrawColor(renderer, 96, 96, 96, 255);

  // Clear window
  SDL_RenderClear(renderer);

  // Get the currentPath

  char cwd[PATH_MAX+1];

  getcwd(cwd, sizeof(cwd));


  //strcat("/res/Robot-Black.ttf", cwd); - Not working don't know why probably not a pointer



  // Render Text??

  // Load font style and set size
  TTF_Font* curdle_font = TTF_OpenFont("../res/Roboto-Black.ttf", 24);

  // Load custom text colour
  SDL_Color curdle_font_colour = {CURDLE_TEXT_COLOUR_R, CURDLE_TEXT_COLOUR_G, CURDLE_TEXT_COLOUR_B, 255};

  // Create a surface to render the text onto?
  SDL_Surface* surface_text = TTF_RenderText_Solid( curdle_font, "hello world", curdle_font_colour);

  // Convert the surface into a texture
  SDL_Texture* texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);

  // Create a rectangle to render the text to
  SDL_Rect text_rect;
  text_rect.x = 0;
  text_rect.y = 0;
  text_rect.w = 600;
  text_rect.h = 100;

  SDL_RenderCopy(renderer, texture_text, NULL, &text_rect);

  draw_rect(renderer);

  SDL_FreeSurface(surface_text);
  SDL_DestroyTexture(texture_text);




  event_poll(window, renderer);


  // Close and destroy the window and renderer
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  TTF_Quit();

  // Call SDL quite before application closes to safely shut down subsystems
  SDL_Quit();

  return EXIT_SUCCESS;
}

int event_poll(SDL_Window* window, SDL_Renderer* renderer) {

  // Create our event
  SDL_Event event;

  while (1) {
    // Poll events
    if (SDL_PollEvent(&event)) {
      // If we have an event

      if (event.type == SDL_QUIT) {
        // Break out of our loop
        break;
      } else if (event.type == SDL_KEYDOWN) {
        // Game logic goes here

          if (event.key.keysym.sym == SDLK_KP_ENTER) {
            // Send the 'enter' to game logic

          }else if (event.key.keysym.sym >= SDL_SCANCODE_A && event.key.keysym.sym <= SDL_SCANCODE_Z) {
            // Send key sym to game logic

          }



        // Ask for the latest current word and update the graphics accordingly
      }
    }


    // Render Logic Goes Here


    // Rendering goes here
  }


}




void draw_rect(SDL_Renderer* renderer) {
  SDL_Rect rectangle;

  rectangle.x = 260;
  rectangle.y = 260;
  rectangle.w = 160;
  rectangle.h = 160;

  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

  SDL_RenderFillRect(renderer, &rectangle);

  SDL_RenderPresent(renderer);

  // Render the rectangle to the screen

}
