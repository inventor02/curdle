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
    .word = word,
    .current_guess = calloc(CURDLE_WORD_LENGTH, sizeof(char)),
  };
  return game;
}

void game_destroy(struct game *game) {
  assert(game != NULL);
  assert(game->guesses != NULL);
  assert(game->current_guess != NULL);

  free(game->guesses);
  game->guesses = NULL;

  free(game->current_guess);
  game->current_guess = NULL;

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

  char *guess_text = calloc(CURDLE_WORD_LENGTH, sizeof(char));

  // TODO: FREE UP ALL OF THESE ADDRESSES AT THE END OF THE PROGRAM
  strncpy(guess_text, current_guess, CURDLE_WORD_LENGTH);
  struct guess guess = {
    .guess_scoring = guess_scoring,
    .guessed_word = guess_text
  };
  return guess;
}

void append_letter(struct game *game, char current_letter){
  printf("append_letter\n");
  printf("Current Letter: %c\n", current_letter);
  printf("Current Guess: %s\n", game->current_guess);
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
    printf("Current Loop: %i\n", i);
    printf("Current Guess: %c\n", game->current_guess[i]); // SEGFAULT whenever current_guess is accessed
    if(game->current_guess[i] == 0){
      if(i < 6){
        game->current_guess[i] = current_letter;
        break;
      }
    }
  }
}

void reset_guess(struct game *game){
  printf("reset_guess\n");
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
    game->current_guess[i] = 0;
  }
}

void append_guess(struct game *game){
  printf("append_guess\n");
  game->guesses[game->guesses_so_far] = guess_comparator(game);
}

void backspace(struct game *game){
  printf("backspace\n");
  if(game->current_guess[0] != 0){
    for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
      if(game->current_guess[i] == 0){
        game->current_guess[i-1] = 0;
        break;
      }
    }
  }
}

void check_game_state(struct game *game){
  printf("%s\n", game->current_guess);
  /// Checks if the right word was guessed
  if(strcmp(game->current_guess, game->word) == 0){
    printf("you won\n");
    //end_game(true);
  }
  /// Checks if all
  if(game->guesses_so_far > CURDLE_MAX_GUESSES){
    //end_game(false);
    printf("end game\n");
  }
  /// Check if the guess is in the guess list
  if(is_valid_guess(game->current_guess)){
    printf("valid entry\n");
    game->guesses_so_far++;
  }  else {
    printf("invalid entry\n");
    // DO SOMETHING TO TELL USER
  }


}
