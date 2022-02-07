/**
 * @file logging.h
 * @brief Function prototypes for the logging logic
 * @author George Peppard
 */

#ifndef CURDLE_LOGGING_H
#define CURDLE_LOGGING_H

#define clogf(level, format, ...) __curdle_log(__func__, level, format, __VA_ARGS__)
#define clog(level, message) __curdle_log_simple(__func__, level, message)

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

void __curdle_log_simple(char const *caller_func_name, enum log_level level, char *string);

void logging_destroy();

#endif
