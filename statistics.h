#ifndef CURDLE_STATISTICS_H
#define CURDLE_STATISTICS_H

#include "game.h"

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

// TODO this is deprecated, to be removed in a future release
#if defined(BSD) || defined(__linux__)
#define CURDLE_STATISTICS_FILE_PATH "/tmp/curdle/statistics.curd"
#endif

#if defined(__APPLE__)
#define CURDLE_STATISTICS_FILE_PATH "~/Library/Application Support/curdle/statistics.curd"
#endif

#if defined(_WIN32)
#define CURDLE_STATISTICS_FILE_PATH "%LocalAppData%/curdle/statistics.curd"
#endif
// end deprecation

#define CURDLE_STATISTICS_FILE_NAME "statistics.curd"

struct game_statistics {
  time_t start_time;
  time_t end_time;
  double game_duration_secs;
};

struct average_statistics {
  uint32_t average_time_secs;
  uint32_t guess_number_totals[CURDLE_MAX_GUESSES];
  uint32_t number_of_games_played;
  uint32_t number_of_games_won;
};

bool statistics_init(struct game_statistics *stats);

void statistics_start_game(struct game_statistics *stats);

void statistics_end_game(struct game_statistics *stats,
  struct average_statistics *average_stats,
  uint8_t number_of_guesses, bool was_word_guessed);

void statistics_destroy(struct game_statistics *stats);

#endif
