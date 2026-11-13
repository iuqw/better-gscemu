#ifndef RC_H
#define RC_H

typedef enum {
  RC_SUCCESS = 0,
  RC_ARGCHECK_FAILED = -1,
  RC_ARG_CHIP_INVALID = -2,
  RC_CHIPEMU_FAILED_INIT = -3,
  RC_CHIPEMU_UNSUPPORTED = -4,
  RC_LOGGER_INVALID_TYPE = -5
} Rc;

#endif /* RC_H */
