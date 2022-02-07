/**
 * @file logging.h
 * @brief Function prototypes for the logging logic
 * @author George Peppard
 */

#ifndef CURDLE_LOGGING_H
#define CURDLE_LOGGING_H

#define curdle_log(level, format, ...) __curdle_log(__func__, level, format, __VA_ARGS__)

/** Log levels for each individual logged message. */
enum log_level {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

void logging_init();

void __curdle_log(char const *caller_func_name, enum log_level level, char *format, ...);

void logging_destroy();

#endif
