#ifndef CURDLE_GAME_H
#define CURDLE_GAME_H

#include <stdint.h>

#define CURDLE_MAX_GUESSES 6
#define CURDLE_WORD_LENGTH 5
#define CURDLE_WORD_LIST_LENGTH 200
#define CURDLE_GUESS_LIST_LENGTH 1000

enum guessed_letter_type {
  NOT_IN_WORD,
  IN_WORD_WRONG_PLACE,
  IN_WORD_RIGHT_PLACE
};

struct guess {
  char *guessed_word;
  enum guessed_letter_type *guess_scoring; // the guessed_letter_types for this guess
};

struct game {
  uint8_t guesses_so_far;
  char *word;
  struct guess *guesses;
};

struct game game_init(char *word);

void game_destroy(struct game *game);

struct guess score_word(char *word);

#endif // CURDLE_GAME_H
