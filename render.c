#include "render.h"
#include <unistd.h>
#include <limits.h>
#include "game.h"
#include "words.h"
#include <stdint.h>

// Private method decleration
void draw_guess(struct guess* guess, uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer, TTF_Font* font);
void draw_blank_row(uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer);
void draw_current_guess(uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer, TTF_Font* font, char* word);

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
  TTF_Font* curdle_font = TTF_OpenFont("../res/Roboto-Black.ttf", 18 * CURDLE_WINDOW_SCALE);

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

  /**
   * USED FOR TESTING
   *
   * DELETE LATER!!!!!!!!!!!!!!
   */
  printf("%s\n", game.word);



  SDL_Rect rect;
  SDL_Rect* tile = &rect;

  while (1) {
    // Poll events
    if (SDL_PollEvent(&event)) {
      // If we have an event

      if (event.type == SDL_QUIT) {
        // Break out of our loop
        break;
      } else if (event.type == SDL_KEYDOWN) {

        printf("KEY CODE: %i\n", event.key.keysym.sym);
        // Game logic goes here

          if (event.key.keysym.sym == SDLK_RETURN) {
            printf("enter pressed\n");
            append_guess(game_ptr);
            if(game_ptr->current_guess[5] != 0){
              printf("a");
              check_game_state(game_ptr);
              reset_guess(game_ptr);
            }

          }else if (event.key.keysym.sym >= 97 && event.key.keysym.sym <= 122) {
            printf("letter pressed\n");
            append_letter(game_ptr, key_to_char(event.key.keysym.sym));
          } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
            backspace(game_ptr);
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

    uint8_t currentGuess = game_ptr->guesses_so_far;


    // Loop through the guesses
    for (uint8_t guess = 0; guess < currentGuess; guess++) {
      draw_guess(&game_ptr->guesses[guess], guess, tile, renderer, font);
    }
    // Render the current guess

    draw_current_guess(currentGuess, tile, renderer, font, game_ptr->current_guess);


    currentGuess++;
    // Render the blank rows
    for (uint8_t i = currentGuess; i < 6; i++) {
      draw_blank_row(i, tile, renderer);
    }

    SDL_RenderPresent(renderer);
  }


}

void draw_guess(struct guess* guess, uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer, TTF_Font* font) {
  // Guess structure:
  char* text = (char*)calloc(2, sizeof(char));
  text[1] = '\0';

  for(uint8_t letter = 0; letter < 5; letter++) {
    text[0] = toupper(guess->guessed_word[letter]);
    draw_tile(tile, renderer, guess->guess_scoring[letter], row, letter, text, font);
  }

  // Free up the memory
  free(text);
}

void draw_blank_row(uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer) {
  for (uint8_t i = 0; i < 5; i++) {
    draw_tile(tile, renderer, BLANK, row, i, NULL, NULL);
  }
}

void draw_current_guess(uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer, TTF_Font* font, char* word) {
  uint8_t letter = 0;
  char* text = (char*)calloc(2, sizeof(char));
  text[1] = '\0';
  for (;letter < 5; letter++) {
    if (word[letter] != '\0') {
      text[0] = toupper(word[letter]);
      draw_tile(tile, renderer, WRONG, row, letter, text, font);
    } else {
      break;
    }
  }

  free(text);

  for (;letter < 5; letter++) {
    draw_tile(tile, renderer, BLANK, row, letter, NULL, NULL);
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

  // Figures out where we need to draw the tile, based off of the row and column
  tile->x = (7 + (column * 28)) * CURDLE_WINDOW_SCALE;
  tile->y = (48 + (row * 28)) * CURDLE_WINDOW_SCALE;
  tile->w = 24 * CURDLE_WINDOW_SCALE;
  tile->h = 24 * CURDLE_WINDOW_SCALE;

  // Sets the background colour
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

  // If we are rendering a blank tile then we can ignore rendering text onto it
  if (type == WRONG || type == RIGHT_WRONG_POSITION || type == RIGHT_RIGHT_POSITION) {
    // Render the character on top of the thing
    SDL_Color text_colour = {CURDLE_TEXT_COLOUR_R, CURDLE_TEXT_COLOUR_G, CURDLE_TEXT_COLOUR_B, 255};
    SDL_Surface* surface_text = TTF_RenderText_Solid(font, letter, text_colour);
    SDL_Texture* texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);

    // Adjust the bounds of the letter so it doesn't stretch it out over the entire tile which looks ugly

    int w, h;
    uint16_t diff_x, diff_y;
    float aspect_ratio;

    if (TTF_SizeText(font, letter, &w, &h)) {
      printf("Error 'rendering' the string: %s\n", SDL_GetError());
    } else {

      diff_x = tile->w - w;
      diff_y = tile->h - h;

    }

    tile->x += diff_x / 2;
    tile->y -= diff_y / 2;
    tile->w -= diff_x;
    tile->h += diff_y;

    SDL_RenderCopy(renderer, texture_text, NULL, tile);


    // Free stuff up here?
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(texture_text);
  }

}
