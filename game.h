#ifndef CURDLE_GAME_H
#define CURDLE_GAME_H

#include <stdint.h>

#define CURDLE_EPOCH 1643068800
#define CURDLE_DAY_LENGTH 86400

#define CURDLE_MAX_GUESSES 6
#define CURDLE_WORD_LENGTH 5
#define CURDLE_WORD_LIST_LENGTH 2315
#define CURDLE_GUESS_LIST_LENGTH 10657

enum guessed_letter_type {
  NOT_IN_WORD,
  IN_WORD_WRONG_PLACE,
  IN_WORD_RIGHT_PLACE
};

struct guess {
  enum guessed_letter_type *guess_scoring; // the guessed_letter_types for this guess
  char *guessed_word;
};

struct game {
  uint8_t guesses_so_far;
  char *word;
  char *valid_guesses;
  struct guess *guesses;
};

char *get_word();

struct game game_init(char word[]);

void game_destroy(struct game *game);

struct guess guess_handler(struct game *game);

uint32_t get_current_word_index();

#endif // CURDLE_GAME_H
