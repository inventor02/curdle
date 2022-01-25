#include <stdlib.h>
#include <assert.h>

#include "game.h"

/**
 * Creates a new game structure given the word that needs to be guessed.
 *
 * @param word the word that needs to be guessed
 * @return the new game structure
 */
struct game game_init(char *word) {
  struct game game = {
    .guesses = calloc(CURDLE_MAX_GUESSES, sizeof(struct guess)),
    .guesses_so_far = 0,
    .word = word
  };

  return game;
}

void game_destroy(struct game *game) {
  assert(game != NULL);
  assert(game->guesses != NULL);

  free(game->guesses);

}

struct guess guess_handler(struct game *game){

}

