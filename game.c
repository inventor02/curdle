#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "game.h"

int8_t index_of(char *haystack, char needle) {
  char *occurrence = strchr(haystack, needle);

  if (occurrence == NULL) {
    return -1;
  } else {
    return haystack - occurrence;
  }
}

/**
 * Creates a new game structure given the word that needs to be guessed.
 *
 * @param word the word that needs to be guessed
 * @return the new game structure
 */
struct game game_init(char word[]) {
  struct game game = {
    .guesses = calloc(CURDLE_MAX_GUESSES, sizeof(struct guess)),
    .guesses_so_far = 0,
    .word = *word,
    .valid_guesses = calloc(CURDLE_GUESS_LIST_LENGTH * CURDLE_WORD_LENGTH, sizeof(char))
  };

  FILE *fp = fopen("possible_guesses.txt", "r");
  assert(fp != NULL);



  return game;
}

void game_destroy(struct game *game) {
  assert(game != NULL);
  assert(game->guesses != NULL);

  free(game->guesses);
  game->guesses = NULL;

  free(game);
}

uint32_t get_current_word_index() {
  uint32_t index = floor((time(NULL) - CURDLE_EPOCH) / CURDLE_DAY_LENGTH);
  return index;
}

struct guess guess_comparator(struct game *game, char *current_guess){
  char *word = game->word;
  enum guessed_letter_type *guess_scoring = calloc(CURDLE_WORD_LENGTH, sizeof(enum guessed_letter_type));
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++) {
    int8_t index = index_of(word, current_guess[i]);
    if(index == -1){
      guess_scoring[i] = NOT_IN_WORD;
    } else if (word[index] == current_guess[i]){
      guess_scoring[i] = IN_WORD_RIGHT_PLACE;
    } else {
      guess_scoring[i] = IN_WORD_WRONG_PLACE;
    }
  }
  struct guess guess = {
    .guess_scoring = guess_scoring,
    .guessed_word = current_guess
  };
  return guess;
}

