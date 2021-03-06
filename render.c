#include "render.h"
#include <unistd.h>
#include <limits.h>
#include "game.h"
#include "statistics.h"
#include "words.h"
#include <stdint.h>
#include "logging.h"

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
    clogf(FATAL, "Error initialising SDL: %s\n", SDL_GetError());
    return CURDLE_SDL_INITILISATION_FAILURE;
  }

  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  // Create a window centered on the screen with set width and height
  window = SDL_CreateWindow("Curdle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 150 * CURDLE_WINDOW_SCALE, 272 * CURDLE_WINDOW_SCALE, 0);

  if (window == NULL) {
    clogf(FATAL, "Error creating window: %s\n", SDL_GetError());
    return CURDLE_SDL_WINDOW_FAILURE;

  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    clogf(FATAL, "Error creating renderer: %s\n", SDL_GetError());
    return CURDLE_SDL_RENDERER_FAILURE;
  }

  // Use Blending
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


  // Get the currentPath

  char cwd[PATH_MAX+1];

  getcwd(cwd, sizeof(cwd));


  //strcat("/res/Robot-Black.ttf", cwd); - Not working don't know why probably not a pointer




  // Render Text??

  // Load font style and set size
  TTF_Font* curdle_font = TTF_OpenFont("../res/Roboto-Black.ttf", 18 * CURDLE_WINDOW_SCALE);

  // Load custom text colour
  SDL_Color curdle_font_colour = {CURDLE_TEXT_COLOUR_R, CURDLE_TEXT_COLOUR_G, CURDLE_TEXT_COLOUR_B, 255};

  // Create Logo Texture

  SDL_Surface* logo_surface = IMG_Load("../res/Curdle.png");
  SDL_Texture* logo_texture = SDL_CreateTextureFromSurface(renderer, logo_surface);




  uint8_t render_status = event_poll(window, renderer, curdle_font, logo_texture);
  if (render_status != 0) {
    return render_status;
  }


  // Close and destroy the window and renderer
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  // Free up resources
  SDL_FreeSurface(logo_surface);
  SDL_DestroyTexture(logo_texture);

  // Close the font
  TTF_CloseFont(curdle_font);

  TTF_Quit();

  // Close the image handling
  IMG_Quit();

  // Call SDL quite before application closes to safely shut down subsystems
  SDL_Quit();

  return EXIT_SUCCESS;
}

int event_poll(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* logo_texture) {

  // Create our event
  SDL_Event event;
  struct game game = game_init(get_today_word());
  struct game *game_ptr = &game;
  clear_alphabet_scoring(game_ptr);

  // Initialise the statistics object

  struct game_statistics game_stats;
  struct game_statistics* game_stats_ptr = &game_stats;

  if (!statistics_init(game_stats_ptr)) {
    clog(FATAL, "Error initialising statistics");
    return CURDLE_STATISTICS_FILE_ERROR;
  }


  char buffer[1024];

  /**
   * USED FOR TESTING
   *
   * DELETE LATER!!!!!!!!!!!!!!
   */
  clogf(DEBUG, "word is: %s", game.word);



  SDL_Rect rect;
  SDL_Rect* tile = &rect;

  uint8_t endgame_animation_status = STOPPED;
  uint8_t endgame_animation_alpha = 0;
  uint32_t endgame_animation_last_update = SDL_GetTicks();

  while (1) {
    // Poll events
    if (SDL_PollEvent(&event)) {
      // If we have an event

      if (event.type == SDL_QUIT) {
        // Break out of our loop
        break;
      } else if (event.type == SDL_KEYDOWN) {

          if (event.key.keysym.sym == SDLK_ESCAPE) {
              // Close the game

              // TODO: Handle any game logic to do with saving here in the future

              break;
          }

          if(!game_ptr->game_ended){
            clogf(DEBUG, "KEY CODE: %i", event.key.keysym.sym);

            // Game logic goes here
            if (event.key.keysym.sym == SDLK_RETURN) {
                append_guess(game_ptr);
                if(game_ptr->guesses_so_far == 0){
                    statistics_start_game(game_stats_ptr);
                }
                if(game_ptr->current_guess[CURDLE_WORD_LENGTH-1] != 0){
                  check_game_state(game_ptr);
                  if(!game_ptr->game_ended){
                    reset_guess(game_ptr);
                  } else {
                    struct average_statistics average_stats;
                    statistics_end_game(game_stats_ptr, &average_stats, game_ptr->guesses_so_far, game_ptr->game_won);
                    clogf(INFO, "Average time: %d", average_stats.average_time_secs);
                    clogf(INFO, "Number of games won: %d", average_stats.number_of_games_won);
                  }
                }

              } else if (event.key.keysym.sym >= 97 && event.key.keysym.sym <= 122) {
                  append_letter(game_ptr, key_to_char(event.key.keysym.sym));
              } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                  backspace(game_ptr);
              }
          }
      }
        // Ask for the latest current word and update the graphics accordingly
    }

    // Render Logic Goes Here



    // Rendering goes here

    // Set background colour
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear window
    SDL_RenderClear(renderer);


    // Render the logo //

    // Transform the rect to the correct position
    tile->x = 0;
    tile->y = 2 * CURDLE_WINDOW_SCALE;
    tile->w = 150 * CURDLE_WINDOW_SCALE;
    tile->h = 50 * CURDLE_WINDOW_SCALE;

    SDL_RenderCopy(renderer, logo_texture, NULL, tile);


    /////////////////////

    uint8_t currentGuess = game_ptr->guesses_so_far;

    // Loop through the guesses
    for (uint8_t guess = 0; guess < currentGuess; guess++) {
      draw_guess(&game_ptr->guesses[guess], guess, tile, renderer, font);
    }
    // Render the current guess
    if (currentGuess < CURDLE_MAX_GUESSES) {
      draw_current_guess(currentGuess, tile, renderer, font, game_ptr->current_guess);
    }

    currentGuess++;
    // Render the blank rows
    for (uint8_t i = currentGuess; i < CURDLE_MAX_GUESSES; i++) {
      draw_blank_row(i, tile, renderer);
    }

    // If the game has just ended start the animation
    if (game_ptr->game_ended && endgame_animation_status == STOPPED && endgame_animation_alpha ==0) {
      endgame_animation_status = IN_PROGRESS;
      endgame_animation_last_update = SDL_GetTicks();
      clogf(DEBUG, "Animation Started - Time: %i", endgame_animation_last_update);
    }

    // handle the animation stuff
    if (endgame_animation_status == IN_PROGRESS) {
      // We want the animation to take roughly 1 second, which is 1000 milliseconds
      // During this time we want to increase a value from 0 to 192
      // This means we want to update this value roughly every 1000 / 192 milliseconds
      // Which is approximately 1000 / 200 which is every 5 milliseconds
      uint32_t current_tick = SDL_GetTicks();
      if (current_tick >= endgame_animation_last_update + 5) {
        // Figure out how many frames of animation to jump due to lower refresh rate monitors

        endgame_animation_alpha += (current_tick - endgame_animation_last_update) / 5;
        // printf("Tick (%i), Time taken for tick: %i\n", endgame_animation_alpha, current_tick - endgame_animation_last_update);
        endgame_animation_last_update = current_tick;
        // If we have reached our desired value then we want to stop the animation
        if (endgame_animation_alpha >= 192) {
          endgame_animation_alpha = 192;
          endgame_animation_status = STOPPED;
          clogf(DEBUG, "Animation Ended - Time: %i", endgame_animation_last_update);
        }
      }
    }

    if (game_ptr->game_ended && endgame_animation_alpha > 0) {

      // Render the statistics gui
      // More tile abuse


      // Darken the background
      tile->x = 0;
      tile->y = 0;
      tile->w = 150 * CURDLE_WINDOW_SCALE;
      tile->h = 240 * CURDLE_WINDOW_SCALE;
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, endgame_animation_alpha);

      SDL_RenderFillRect(renderer, tile);


      // Render the statistics
      // Create a colour for the text
      SDL_Color text_colour = {255, 255, 255, 255};

      snprintf(buffer, sizeof(buffer), "Time: %lfs", game_stats_ptr->game_duration_secs);
      // Create the relevant surface and texture to render the text to screen
      SDL_Surface* text_surface = TTF_RenderText_Solid(font, buffer, text_colour);
      SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

      // TODO: Render the text somewhere

      // Free up texture and surface once we are done
      SDL_DestroyTexture(text_texture);
      SDL_FreeSurface(text_surface);

    }

    draw_small_tile(tile, renderer, RIGHT_WRONG_POSITION, 7, 220, game_ptr->word, font);

    SDL_RenderPresent(renderer);
  }

  statistics_destroy(game_stats_ptr);

  return 0;


}

void draw_guess(struct guess* guess, uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer, TTF_Font* font) {
  // Guess structure:
  char* text = (char*)calloc(2, sizeof(char));
  text[1] = '\0';

  for(uint8_t letter = 0; letter < CURDLE_WORD_LENGTH; letter++) {
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
  tile->y = (52 + (row * 28)) * CURDLE_WINDOW_SCALE;
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

    if (TTF_SizeText(font, letter, &w, &h)) {
      clogf(ERROR, "Error 'rendering' the string: %s\n", SDL_GetError());
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

void draw_small_tile(SDL_Rect* tile, SDL_Renderer* renderer, enum rectangle_draw_type type, uint16_t x, uint16_t y, char* letter, TTF_Font* font) {
  // TODO: Implement this function
  //
  // Width: 171 / 10 = 17
  // Height: 40 / 3 = 13

  tile->x = x * CURDLE_WINDOW_SCALE;
  tile->y = y * CURDLE_WINDOW_SCALE;
  tile->h = 17 * CURDLE_WINDOW_SCALE;
  tile->w = 13 * CURDLE_WINDOW_SCALE;

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

}
