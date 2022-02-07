/*
statistics.curd file format
========================================

[average time HH]
[average time MM]
[average time SS]
[guess number total 1],[guess number total 2],etc
[number of games played]
[number of games won]
 */

#include "statistics.h"

#include "game.h"

#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>

struct average_statistics default_stats = {
  0,
  0,
  0,
  0,
};

char *stats_file_path;

/**
 * Prints a log message to STDOUT.
 *
 * @param message  the message to display
 */
void stats_log(char *message) {
  printf("[curdle:statistics] %s\n", message);
}

bool init_stats_file_path() {
  stats_log("initialise the path to the stats file");
  stats_file_path = calloc(FILENAME_MAX, sizeof(char));

  // We check the home directory location depending on the operating system.
  // The char *file_path must always be set after every execution.
  // If nothing works, then that is very sad, and we will return false.
  // If it does work, we will set the global char *stats_file_path and return true.
  // Why is this so complex!?!??!?!?!?

#if defined(__APPLE__)
  stats_log("running in apple mode");
  // TODO implement this
#endif

#if defined(__linux__) || defined(BSD)
  stats_log("running in linux mode");

  stats_log("try and use HOME environment variable");
  char *file_path = getenv("HOME");

  // the HOME environment variable is not always set
  if (file_path == NULL) {
    stats_log("HOME environment variable not set so read passwd");
    file_path = getpwuid(getuid())->pw_dir;
  }

  // if home_dir is still NULL then give up
  if (file_path == NULL) {
    stats_log("unable to get a home directory, giving up");
    return false;
  }

  char *stats_file_name = CURDLE_STATISTICS_FILE_NAME;

  strcat(file_path, "/.curdle/");

  // Try and create the directory for the statistics files
  // If the folder does not exist, and creation is successful, we're good
  // If the folder does not exist and can't be created then this sets an error code, which we could
  // catch, but it will fail later anyway
  // If the folder does exist, this also sets an error code, but the rest of it will work fine so
  // there is no need to do anything
  mkdir(file_path, 0700);

  strcat(file_path, stats_file_name);

  stats_log("full file path is");
  printf("%s\n", file_path);
#endif

#if defined(_WIN32)
  stats_log("running in windows mode");
  // TODO implement this
#endif

  strcpy(stats_file_path, file_path);

  stats_log("path to file initialised");
  printf("%s\n", stats_file_path);
  return true;
}

/**
 * Creates the statistics file if it does not exist.
 *
 * @return whether the operation was successful
 */
bool create_file_if_not_exists() {
  if (access(stats_file_path, R_OK|W_OK) == 0) {
    stats_log("file exists, check permissions");
    // TODO check the file is in a valid format
    stats_log("file exists, all good");
    return true;
  } else {
    stats_log("file does not exist, try create");

    FILE *fp = fopen(stats_file_path, "w+");

    if (fp == NULL) {
      stats_log("FATAL: cannot create file");
      printf("%s\n", strerror(errno));
      return false;
    }

    if (fwrite(&default_stats, sizeof(struct average_statistics), 1, fp) != 1) {
      stats_log("FATAL: wrong number of items written");
      return false;
    }

    fclose(fp);
    fp = NULL;

    stats_log("written a default file");
    return true;
  }
}

/**
 * Gets the average statistics from the statistics file in the FS.
 *
 * @return the average statistics of this user
 */
struct average_statistics get_average_statistics_from_fs() {
  stats_log("trying to open file to get stats");

  FILE *fp = fopen(stats_file_path, "r+");

  if (fp == NULL) {
    stats_log("FATAL: cannot open statistics file");
    return default_stats;
  }

  struct average_statistics *in = malloc(sizeof(struct average_statistics));
  assert (in != NULL);

  if (fread(in, sizeof(struct average_statistics), 1, fp) != 1) {
    stats_log("FATAL: wrong number of items read");
    return default_stats;
  }

  struct average_statistics stats = *in;

  free(in);
  in = NULL;

  return stats;
}

bool save_average_statistics_to_fs(struct average_statistics *stats) {
  stats_log("try open file to save");

  FILE *fp = fopen(stats_file_path, "w+");

  if (fp == NULL) {
    stats_log("FATAL: cannot open file");
    return false;
  }

  if (fwrite(stats, sizeof(struct average_statistics), 1, fp) != 1) {
    stats_log("invalid no of stats written");
  }
}

/**
 * Initialises the game statistics logic.
 *
 * Initialises the statistics structure specified by the parameters, and also
 * checks that the required files exist. If they do not, they will be created.
 *
 * @param stats  the statistics structure to initialise
 * @returns      whether the logic was initialised successfully
 */
bool statistics_init(struct game_statistics *stats) {
  stats_log("hello, init statistics");

  init_stats_file_path();

  if (!create_file_if_not_exists()) {
    return false;
  }

  stats_log("put initial struct");
  *stats = (struct game_statistics) {
    0, 0, 0
  };

  stats_log("stats init done");
  return true;
}

/**
 * Records the start time of the game.
 *
 * The game starts when the first guess has been submitted.
 *
 * @param stats  the statistics structure to record data in
 */
void statistics_start_game(struct game_statistics *stats) {
  time_t now = time(NULL);
  stats->start_time = now;
}

/**
 * Records the end time of the game, calculates stats, and stores them.
 *
 * @param stats              the game statistics struct
 * @param average_stats      the average statistics struct
 * @param number_of_guesses  the number of guesses taken
 * @param was_word_guessed   whether the word was guessed
 */
void statistics_end_game(struct game_statistics *stats,
  struct average_statistics *average_stats,
  uint8_t number_of_guesses, bool was_word_guessed) {
  stats_log("game ended");

  *average_stats = get_average_statistics_from_fs();

  time_t now = time(NULL);
  stats->end_time = now;

  stats->game_duration_secs = difftime(now, stats->start_time);

  stats_log("work out averages");

  average_stats->number_of_games_played++;

  if (was_word_guessed) {
    average_stats->number_of_games_won++;
  }

  average_stats->guess_number_totals[number_of_guesses - 1]++;

  average_stats->average_time_secs = ((average_stats->average_time_secs *
    average_stats->number_of_games_played) + stats->game_duration_secs) /
    average_stats->number_of_games_played;

  stats_log("try and save");
  if(!save_average_statistics_to_fs(average_stats)) {
    stats_log("cannot save!");
  }
  stats_log("saved stats");
}

/**
 * Frees up memory used by the statistics module, ready for the program to exit.
 *
 * @param stats  the statistics structure
 */
void statistics_destroy(struct game_statistics *stats) {
  free(stats_file_path);
  stats_file_path = NULL;
}
