#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "game.h"

/**
 * A method to return the index of the first occurance of a specific letter in a string.
 * @param haystack is the string in which to search for the needle
 * @param needle is the character to search for
 * @return the index of the first occurance of the letter
 */
int8_t index_of(char *haystack, char needle) {
  char *occurrence = strchr(haystack, needle);

  if (occurrence == NULL) {
    return -1;
  } else {
    return haystack - occurrence;
  }
}

/**
 * A method to open the word list and find the word for today using a pre-existing formula.
 * @return the word for today
 */
char *get_word() {
  FILE *fp = fopen("words.txt", "r");
  assert(fp != NULL);

  char *words = calloc(CURDLE_WORD_LENGTH * CURDLE_WORD_LIST_LENGTH, sizeof(char));
  uint32_t index = 0;
  char *current_word;
  while (fgets(current_word, CURDLE_WORD_LENGTH + 1, fp)) {
    words[CURDLE_WORD_LENGTH * index] = *current_word;
    index++;
  }

  char *word_of_today = malloc((CURDLE_WORD_LENGTH + 1) * sizeof(char));
  strcpy(word_of_today, (words + get_current_word_index() * CURDLE_WORD_LENGTH));
  free(words);
  words = NULL;

  return word_of_today;
}

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
    .word = word,
    .valid_guesses = calloc(CURDLE_GUESS_LIST_LENGTH * CURDLE_WORD_LENGTH, sizeof(char))
  };

  FILE *fp = fopen("possible_guesses.txt", "r");
  assert(fp != NULL);





  // WTF does the following do?
  char *the_word;
  uint32_t index = 0;
  while (fgets(the_word, CURDLE_WORD_LENGTH+1, fp)) {
    game.valid_guesses[index * CURDLE_WORD_LENGTH] = *the_word;
    index++;
  }

  return game;
}

void game_destroy(struct game *game) {
  assert(game != NULL);
  assert(game->guesses != NULL);

  free(game->guesses);
  game->guesses = NULL;

  free(game);
}

/**
 * Calculate the index of todays word.
 * @return the index of today's word
 */
uint32_t get_current_word_index() {
  uint32_t index = floor((time(NULL) - CURDLE_EPOCH) / CURDLE_DAY_LENGTH);
  return index;
}

/**
 * Comparator for each letter of the guess and returns a guess struct.
 *
 * @param game is the current game
 * @param current_guess is the guess just entered and the guess to compare
 * @return the guess struct with each letter scored
 */
struct guess guess_comparator(struct game *game, char *current_guess) {
  char *word = game->word;
  enum guessed_letter_type *guess_scoring = calloc(CURDLE_WORD_LENGTH, sizeof(enum guessed_letter_type));
  for (uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++) {
    int8_t index = index_of(word, current_guess[i]);
    if (index == -1) {
      guess_scoring[i] = NOT_IN_WORD;
    } else if (word[index] == current_guess[i]) {
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
