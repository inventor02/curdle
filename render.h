#ifndef CURDLE_RENDER_H
#define CURDLE_RENDER_H

#include <SDL2/SDL.h>

#define CURDLE_WINDOW_SCALE 4

#define CURDLE_TEXT_COLOUR_R 255
#define CURDLE_TEXT_COLOUR_G 255
#define CURDLE_TEXT_COLOUR_B 255

#define CURDLE_TEXT_FONT "./res/Roboto-Black.ttf"
#define CURDLE_TEXT_SIZE 96

#define CURDLE_SDL_INITILISATION_FAILURE 1
#define CURDLE_SDL_WINDOW_FAILURE 2
#define CURDLE_SDL_RENDERER_FAILURE 3


/**
 * Initialises SDL and draws to the window.
 *
 * @return int error code.
 */
int start_window();

void draw_rect(SDL_Renderer* renderer);

int event_poll(SDL_Window* window, SDL_Renderer* renderer);

#endif // Curdle Render Header
