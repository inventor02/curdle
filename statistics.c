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

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

struct average_statistics default_stats = {
  0,
  0,
  0,
  0,
};

/**
 * Prints a log message to STDOUT.
 *
 * @param message  the message to display
 */
void stats_log(char *message) {
  printf("[curdle:statistics] %s\n", message);
}

/**
 * Creates the statistics file if it does not exist.
 *
 * @return whether the operation was successful
 */
bool create_file_if_not_exists() {
  stats_log("statistics at:");
  stats_log(CURDLE_STATISTICS_FILE_PATH);
  if (access(CURDLE_STATISTICS_FILE_PATH, F_OK)) {
    stats_log("file exists, check permissions");

    if (access(CURDLE_STATISTICS_FILE_PATH, R_OK|W_OK)) {
      stats_log("FATAL: cannot read/write statistics file");
      return false;
    }

    // TODO check the file is in a valid format
    stats_log("file exists, all good");
    return true;
  } else {
    stats_log("file does not exist, try create");

    FILE *fp = fopen(CURDLE_STATISTICS_FILE_PATH, "r+");

    if (fp == NULL) {
      stats_log("FATAL: cannot create file");
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

  FILE *fp = fopen(CURDLE_STATISTICS_FILE_PATH, "r+");

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

char *encode_bytes(char *bytes) {
  // TODO implement this
  return bytes;
}

char *decode_bytes(char *bytes) {
  // TODO implement this
  return bytes;
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

}
