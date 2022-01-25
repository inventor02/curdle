#include <stdlib.h>
#include "game.h"

struct game game_init(char *word) {
  struct game game = {
      .guesses = calloc(CURDLE_MAX_GUESSES, sizeof(struct guess))

  };
}
