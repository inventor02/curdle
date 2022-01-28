//
// Created by george on 28/01/2022.
//

#ifndef CURDLE_WORDS_H
#define CURDLE_WORDS_H

#include <stdbool.h>

#define CURDLE_WORD_LENGTH 5
#define CURDLE_WORD_LIST_LENGTH 2315
#define CURDLE_GUESS_LIST_LENGTH 10657

extern const char *words[];
extern const char *possible_guess_words[];

const char *get_today_word();

bool is_valid_guess(char *word);

#endif //CURDLE_WORDS_H
