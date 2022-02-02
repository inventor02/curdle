//
// Created by george on 28/01/2022.
//

#ifndef CURDLE_WORDS_H
#define CURDLE_WORDS_H

#include <stdbool.h>

#define CURDLE_WORD_LENGTH 5
#define CURDLE_WORD_LIST_LENGTH 2315
#define CURDLE_GUESS_LIST_LENGTH 10657

char *get_today_word();

bool is_valid_guess(char *word);

void score_guess(enum guessed_letter_type *scoring_ptr, char *guess, char *correct_word);

#endif //CURDLE_WORDS_H
