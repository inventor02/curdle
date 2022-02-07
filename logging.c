/**
 * @file logging.c
 * @brief Utility functions for logging data to the console
 * @author George Peppard
 */

#include "logging.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

char *level_to_str(enum log_level level) {
  switch (level) {
    case DEBUG: return "DEBUG";
    case INFO: return "INFO";
    case WARN: return "WARN";
    case ERROR: return "ERROR";
    case FATAL: return "FATAL";
    default: return "DEBUG(D)";
  }
}

void __curdle_log(char const *caller_func_name, enum log_level level, char *format, ...) {
  printf("[%s:curdle:%s] ", level_to_str(level), caller_func_name);

  va_list argp;
  va_start(argp, format);
  vfprintf(stdout, format, argp);
  va_end(argp);

  printf("\n");
}

