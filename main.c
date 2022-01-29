#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>
#include "game.h"
#include "words.h"
#include "render.h"

int main() {

  start_window();

  struct game this_game = game_init(get_today_word());

  return 0;
}
