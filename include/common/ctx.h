#ifndef CTX_H
#define CTX_H

#include <stdbool.h>
#include <unicorn/unicorn.h>

#include <libargs.h>
#include <common/emulator.h>

typedef struct {
  ArgCheck *ac;
  EmulatorChips chip;
  Emulator chipemu;
  uc_engine *uc;
  bool verbose;
} Ctx;

extern Ctx ctx;

void ctx_init(void);

#endif /* CTX_H */
