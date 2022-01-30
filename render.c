#include "render.h"
#include <unistd.h>
#include <limits.h>
#include "game.h"
#include "words.h"

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
  window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 150 * CURDLE_WINDOW_SCALE, 240 * CURDLE_WINDOW_SCALE, 0);

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


  // Get the currentPath

  char cwd[PATH_MAX+1];

  getcwd(cwd, sizeof(cwd));


  //strcat("/res/Robot-Black.ttf", cwd); - Not working don't know why probably not a pointer



  // Render Text??

  // Load font style and set size
  TTF_Font* curdle_font = TTF_OpenFont("../res/Roboto-Black.ttf", 256);

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




  event_poll(window, renderer, curdle_font);


  // Close and destroy the window and renderer
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  // Close the font
  TTF_CloseFont(curdle_font);

  TTF_Quit();

  // Call SDL quite before application closes to safely shut down subsystems
  SDL_Quit();

  return EXIT_SUCCESS;
}

int event_poll(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {

  // Create our event
  SDL_Event event;
  struct game game = game_init(get_today_word());
  struct game *game_ptr = &game;

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
            guess_comparator(game_ptr);
            reset_guess(game_ptr);

          }else if (event.key.keysym.sym >= SDL_SCANCODE_A && event.key.keysym.sym <= SDL_SCANCODE_Z) {
            append_letter(game_ptr, key_to_char(event.key.keysym.sym));
          }



        // Ask for the latest current word and update the graphics accordingly
      }
    }


    // Render Logic Goes Here



    // Rendering goes here

    // Set background colour
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear window
    SDL_RenderClear(renderer);

    SDL_Rect tile;
    // Render test tile?

    char test_char_y[2] = {'Y','\0'};
    char test_char_o[2] = {'O', '\0'};
    draw_tile(&tile, renderer, RIGHT_WRONG_POSITION, 0, 0, test_char_y, font);
    draw_tile(&tile, renderer, RIGHT_RIGHT_POSITION, 0, 1, test_char_o, font);
    draw_tile(&tile, renderer, WRONG, 0, 2, test_char_y, font);
    draw_tile(&tile, renderer, WRONG, 0, 3, test_char_o, font);
    draw_tile(&tile, renderer, RIGHT_RIGHT_POSITION, 0, 4, test_char_y, font);

    for (uint8_t row = 1; row < 6; row++) {
      for (uint8_t column = 0; column < 5; column++) {
        draw_tile(&tile, renderer, BLANK, row, column, NULL, NULL);
      }
    }
    //draw_tile(tile, renderer, RIGHT_RIGHT_POSITION, 0, 0, test_char, font);
    SDL_RenderPresent(renderer);
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

void draw_tile(SDL_Rect* tile, SDL_Renderer* renderer, enum rectangle_draw_type type, uint8_t row, uint8_t column, char* letter, TTF_Font* font) {

  // Positioning as follows:
  // 240px of height for aspect ratio of one
  // 6 rows - Make 9 rows with one row of pixels shared between the rows
  // 240px / 10 = 24
  // Rows numbered from 0-5
  
  // Columns
  // 135px / 9 = 15
  // 2 columns either side, with

  tile->x = (7 + (column * 28)) * CURDLE_WINDOW_SCALE;
  tile->y = (48 + (row * 28)) * CURDLE_WINDOW_SCALE;
  tile->w = 24 * CURDLE_WINDOW_SCALE;
  tile->h = 24 * CURDLE_WINDOW_SCALE;

  switch (type)
  {
  case BLANK:
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    break;
  case WRONG:
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    break;
  case RIGHT_WRONG_POSITION:
    SDL_SetRenderDrawColor(renderer, 25, 178, 255, 255);
    break;
  case RIGHT_RIGHT_POSITION:
    SDL_SetRenderDrawColor(renderer, 255, 102, 128, 255);
    break;
  
  default: 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    break;
  }

  SDL_RenderFillRect(renderer, tile);
  // Render the outline of the tile
  SDL_SetRenderDrawColor(renderer, 255, 255, 255 ,255);
  SDL_RenderDrawRect(renderer, tile);

  if (type == WRONG || type == RIGHT_WRONG_POSITION || type == RIGHT_RIGHT_POSITION) {
    // Render the character on top of the thing
    SDL_Color text_colour = {CURDLE_TEXT_COLOUR_R, CURDLE_TEXT_COLOUR_G, CURDLE_TEXT_COLOUR_B, 255};
    SDL_Surface* surface_text = TTF_RenderText_Solid(font, letter, text_colour);
    SDL_Texture* texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);

    // Adjust the bounds of the letter so it doesn't stretch it out over the entire tile which looks ugly

    tile->x += 2 * CURDLE_WINDOW_SCALE;
    tile->y -= 2 * CURDLE_WINDOW_SCALE;
    tile->w -= 4 * CURDLE_WINDOW_SCALE;
    tile->h += 4 * CURDLE_WINDOW_SCALE;

    SDL_RenderCopy(renderer, texture_text, NULL, tile);


    // Free stuff up here?
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(texture_text);
  }

}
