#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "words.h"

#include "logging.h"

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
    .current_guess = calloc(CURDLE_WORD_LENGTH+1, sizeof(char)),
    .game_ended = false,
    .game_won = false,
    .alphabet_scoring = calloc(26, sizeof(enum keyboard_letter_type)),
  };
  return game;
}

void clear_alphabet_scoring(struct game *game){
  for(uint8_t i = 0; i < 26; i++){
    game->alphabet_scoring[i] = NOT_CHECKED;
  }
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
  score_guess(guess_scoring, game->current_guess, game->word);

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
  if(game->current_guess[CURDLE_WORD_LENGTH-1] != '\0'){
    game->current_guess[CURDLE_WORD_LENGTH] = 0;
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
  /*if(strncmp(game->current_guess, game->word, CURDLE_WORD_LENGTH) == 0){
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
  }*/
  if(is_valid_guess(game->current_guess)){
    printf("valid entry\n");
    game->guesses_so_far++;
    printf("guesses_so_far %d\n", game->guesses_so_far);
    score_alphabet(game);
    if(strncmp(game->current_guess, game->word, CURDLE_WORD_LENGTH) == 0){
      game->game_ended = true;
      end_game(game, true);
    } else if(game->guesses_so_far >= CURDLE_MAX_GUESSES){
      game->game_ended = true;
      end_game(game, false);
    }


  } else {
    printf("invalid entry\n");
    // DO SOMETHING TO TELL USER
    reset_guess(game);
  }

}

void score_alphabet(struct game *game){
  struct guess thisGuess = game->guesses[game->guesses_so_far-1];
  printf("\n\nAlphabet Scoring=============\n");
  printf("Current guess: %s\n", thisGuess.guessed_word);
  for(uint8_t i = 0; i < CURDLE_WORD_LENGTH; i++){
    char thisChar = thisGuess.guessed_word[i];
    uint8_t alphabetIndex = thisChar - 'a';
    if(game->alphabet_scoring[alphabetIndex] == NOT_CHECKED){
      game->alphabet_scoring[alphabetIndex] = (enum keyboard_letter_type) thisGuess.guess_scoring[i];
    } else if(game->alphabet_scoring[alphabetIndex] < (enum keyboard_letter_type) thisGuess.guess_scoring[i]){
      game->alphabet_scoring[alphabetIndex] = (enum keyboard_letter_type) thisGuess.guess_scoring[i];
    }
  }
  for(uint8_t i = 0; i < 26; i++){
    printf("%c: %i\n", (char) (i + 'a'), game->alphabet_scoring[i]);
  }
}

void end_game(struct game *game, bool won){
  game->game_won = won;
  printf("guesses_so_far %d\n", game->guesses_so_far);
  if(won){
    printf("You Won!\n");
    reset_guess(game);
  } else {
    printf("You lost\n");
  }
  printf("Game Over\n");
  printf("%d\n", game->guesses_so_far);
}
