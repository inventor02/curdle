#include "statistics.h"

#include "game.h"

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void stats_log(char *message) {
  printf("[statistics.c] %s\n", message);
}

/**
 * Creates the statistics file if it does not exist.
 *
 * @return whether the operation was successful
 */
bool create_file_if_not_exists() {
  stats_log("file checking");
  if (access(CURDLE_STATISTICS_FILE_PATH, F_OK)) {
    stats_log("file exists, check permissions");
    if (access(CURDLE_STATISTICS_FILE_PATH, R_OK|W_OK)) {
      stats_log("FATAL: cannot read/write statistics file");
      return false;
    }
  } else {
    stats_log("file does not exist, try create");

    FILE *fp = fopen(CURDLE_STATISTICS_FILE_PATH, "r+");
    // todo
  }
}

struct average_statistics get_average_statistics_from_fs();

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
