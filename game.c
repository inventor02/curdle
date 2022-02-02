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
    .game_ended = false,
    .game_won = false,
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

  //free(game); // CAUSES double free error
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
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
    if(game->current_guess[i] == 0){
      if(i < 6){
        printf("append_letter\n");
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
  printf("Current Guess: %s\n", game->current_guess);
  if(game->current_guess[CURDLE_WORD_LENGTH-1] != 0){
    game->guesses[game->guesses_so_far] = guess_comparator(game);
  } else {
    reset_guess(game);
  }
}

void backspace(struct game *game){
  printf("backspace\n");
  bool deleted = false;
  if(game->current_guess[0] != 0){
    for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
      if(game->current_guess[i] == 0){
        game->current_guess[i-1] = 0;
        deleted = true;
        break;
      }
    }
    if(!deleted){
      game->current_guess[CURDLE_WORD_LENGTH - 1] = 0;
    }
  }
  printf("New Current Guess: %s\n", game->current_guess);
}

void check_game_state(struct game *game){
  printf("\n\nCurrent Guess: %s\n", game->current_guess);
  printf("%d\n", is_valid_guess(game->current_guess));
  /// Checks if the right word was guessed
  if(strncmp(game->current_guess, game->word, CURDLE_WORD_LENGTH) == 0){
    game->game_ended = true;
    end_game(game, true);
  }
  /// Checks if all guesses have been used
  else if(game->guesses_so_far > CURDLE_MAX_GUESSES){
    game->game_ended = true;
    end_game(game, false);
  }
  /// Check if the guess is in the guess list
  else if(is_valid_guess(game->current_guess)){
    printf("valid entry\n");
    game->guesses_so_far++;
  } else {
    printf("invalid entry\n");
    // DO SOMETHING TO TELL USER
    reset_guess(game);
  }
}

void end_game(struct game *game, bool won){
  game->game_won = won;
  if(won){
    printf("You Won!\n");
  } else {
    printf("You lost\n");
  }
  printf("Game Over\n");
}
