#include <stdarg.h>
#include <stdio.h>

#include <common/ctx.h>
#include <common/exit.h>
#include <common/logger.h>
#include <common/rc.h>

void log_internal(LogType logtype, const char *file, int line, const char *msg, ...) {
  const char *tag = "";
  va_list args;

  switch (logtype) {
    case NORMAL:
      tag = "INFO";
      break;
    case ERROR:
      tag = "ERROR";
      break;
    case VERBOSE:
      if (ctx.verbose) {
        tag = "VERBOSE";
      } else {
        return;
      }
      break;
    default:
      /* Something has gone wrong, halt emulation immediately. */
      printf("ERROR!! Invalid logType passed!");
      emu_exit(RC_LOGGER_INVALID_TYPE);
      break;
  }

  va_start(args, msg);
  printf("[%s] (%s:%d) ", tag, file, line);
  vprintf(msg, args);
  va_end(args);
}
