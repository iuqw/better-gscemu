#include <H1B3C/emulator.h>

#include <unicorn/unicorn.h>

#include <common/ctx.h>
#include <common/emulator.h>
#include <common/exit.h>
#include <common/logger.h>
#include <common/rc.h>

void Havn_Initialize(void) {
  uc_err err;

  err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS, &ctx.uc);
  if (err != UC_ERR_OK) {
    ctx.uc = NULL;
    LOG_ERR("Failed to open unicorn engine: %s\n", uc_strerror(err));
    emu_exit(RC_CHIPEMU_FAILED_INIT);
  }

  LOG_VBS("Opened unicorn engine\n");
  ctx.chipemu.ready = true;
}

void Havn_StartEmulation(void) {
  LOG("started emu\n");
}

void Havn_EndEmulation(void) {
  uc_err err;

  LOG("ended emu\n");

  if (ctx.uc == NULL) return;

  err = uc_close(ctx.uc);
  ctx.uc = NULL;
  if (err != UC_ERR_OK) LOG_ERR("Failed to close unicorn engine: %s\n", uc_strerror(err));
}
