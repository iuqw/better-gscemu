#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
  NORMAL = 0,
  ERROR = 1,
  VERBOSE = 2,
  LOGTYPE_END
} LogType;

#define LOG(msg, ...) log_internal(NORMAL, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_ERR(msg, ...) log_internal(ERROR, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_VBS(msg, ...) log_internal(VERBOSE, __FILE__, __LINE__, msg, ##__VA_ARGS__)


void log_internal(LogType logtype, const char *file, int line, const char *msg, ...);
 
#endif /* LOGGER_H */
