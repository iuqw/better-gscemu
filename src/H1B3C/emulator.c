#include <stdbool.h>
#include <unicorn/unicorn.h>

#include <libargs.h>
#include <common/logger.h>
#include <common/emulator.h>
#include <common/exit.h>
#include <H1B3C/emulator.h>

uc_engine *Havn_Initialize(Emulator *chipemu, ArgCheck *ac) {
  uc_engine *uc = NULL;
  uc_err err;

  err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS, &uc);
  if (err != UC_ERR_OK) {
    LOG_ERR("Failed to open unicorn engine: %s\n", uc_strerror(err));
    emu_exit(-1, ac, chipemu, uc);
  }

  LOG_VBS("Opened unicorn engine\n");
  chipemu->ready = true;
  return uc;
}

void Havn_StartEmulation(uc_engine *uc) {
  LOG("started emu\n");
}

void Havn_EndEmulation(uc_engine *uc) {
  uc_err err;

  LOG("ended emu\n");

  if (!uc) return;

  err = uc_close(uc);
  if (err != UC_ERR_OK) LOG_ERR("Failed to close unicorn engine: %s\n", uc_strerror(err));
}
