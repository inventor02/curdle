#ifndef CURDLE_GAME_H
#define CURDLE_GAME_H

#include <stdint.h>
#include <SDL2/SDL.h>

#define CURDLE_EPOCH 1643068800
#define CURDLE_DAY_LENGTH 86400

#define CURDLE_MAX_GUESSES 6

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
  struct guess *guesses;
  char *current_guess;
};

char key_to_char(SDL_Keycode keycode);

void append_letter(struct game *game, char current_letter);

void reset_guess(struct game *game);

struct game game_init(char *word);

void game_destroy(struct game *game);

void append_guess(struct game *game);

struct guess guess_comparator(struct game *game);

void backspace(struct game *game);

#endif // CURDLE_GAME_H
