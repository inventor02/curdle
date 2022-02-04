#ifndef CURDLE_RENDER_H
#define CURDLE_RENDER_H
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdbool.h>
#include "game.h"
#include "statistics.h"

#define CURDLE_WINDOW_SCALE 4

#define CURDLE_TEXT_COLOUR_R 255
#define CURDLE_TEXT_COLOUR_G 255
#define CURDLE_TEXT_COLOUR_B 255

#define CURDLE_TEXT_FONT "./res/Roboto-Black.ttf"
#define CURDLE_TEXT_SIZE 96

#define CURDLE_LOGO_PATH = "./res/Curdle.png"

#define CURDLE_SDL_INITILISATION_FAILURE 1
#define CURDLE_SDL_WINDOW_FAILURE 2
#define CURDLE_SDL_RENDERER_FAILURE 3
#define CURDLE_STATISTICS_FILE_ERROR 4

enum rectangle_draw_type {
  WRONG,
  RIGHT_WRONG_POSITION,
  RIGHT_RIGHT_POSITION,
  BLANK
};

/**
 * Initialises SDL and draws to the window.
 *
 * @return int error code.
 */
int start_window();


void draw_guess(struct guess* guess, uint8_t row, SDL_Rect* tile, SDL_Renderer* renderer, TTF_Font* font);

/**
 * Draws a simple rectangle to the window - used for testing purposes only
 *
 * @param renderer the renderer to use when drawing the triangle
 */
void draw_rect(SDL_Renderer* renderer);

/**
 * Starts the window and main game loop, which includes rendering, event polling and game logic calls
 *
 * @param window the window to render to
 * @param renderer the renderer we are using to render to the window
 * @param font the font we are using for our tiles
 * @return int the error code of the program
 */
int event_poll(SDL_Window *window, SDL_Renderer *renderer, TTF_Font* font, SDL_Texture* logo_texture);

/**
 * Draws a tile to the specified row and column, taking into account the window scale
 *
 * @param tile a rectangle object to reduce memory usage
 * @param renderer the renderer used to draw the tile
 * @param type the type of letter
 * @param row the row of the tile
 * @param column the column of the tile
 * @param letter the letter inside of the tile, use NULL if drawing a blank tile
 * @param font the font used to render the letter, use NULL if drawing a blank tile
 */
void draw_tile(SDL_Rect* tile, SDL_Renderer* renderer, enum rectangle_draw_type type, uint8_t row, uint8_t column, char* letter, TTF_Font* font);
#endif // Curdle Render Header
