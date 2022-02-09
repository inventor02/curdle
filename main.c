#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>
#include "game.h"
#include "words.h"
#include "render.h"
#include "logging.h"

int main() {
  // We need soup
  logging_init();

  start_window();

  logging_destroy();

  return 0;
}
