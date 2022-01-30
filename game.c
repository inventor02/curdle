#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "words.h"

#include "game.h"

/**
 * Converts an SDL key code into a character.
 *
 * @param  keycode the SDL key code to convert
 * @return         the character the SDL code corresponds to
 */
char key_to_char(SDL_Keycode keycode) {
  return (char) (97 + (keycode - SDLK_a));
}

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
struct guess guess_comparator(struct game *game) {
  char *word = game->word;
  char *current_guess = game->current_guess;
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

void append_letter(struct game *game, char current_letter){
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
    if(game->current_guess[i] == 0){
      game->current_guess[i] = current_letter;
      break;
    }
  }
}

void reset_guess(struct game *game){
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
    game->current_guess[i] = 0;
  }
}

void append_guess(struct game *game, uint8_t guess_number, struct guess this_guess){
  game->guesses[guess_number] = this_guess;
}
