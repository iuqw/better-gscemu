#include <string.h>

#include <H1B3C/emulator.h>
#include <common/ctx.h>
#include <common/emulator.h>
#include <common/exit.h>
#include <common/logger.h>
#include <common/rc.h>
#include <libargs.h>

static void get_chipemu(EmulatorChips chip) {
  switch (chip) {
    case H1B3C:
      ctx.chipemu.initialize = Havn_Initialize;
      ctx.chipemu.start_emulation = Havn_StartEmulation;
      ctx.chipemu.end_emulation = Havn_EndEmulation;
      return;
    default:
      emu_exit(RC_CHIPEMU_UNSUPPORTED);
  }
}

int main(int argc, char **argv) {
  const char *chip;

  ctx_init();

  ctx.ac = argcheck_new(argc, argv);
  if (!ctx.ac) emu_exit(RC_ARGCHECK_FAILED);

  argcheck_set_description(ctx.ac, "gscemulator ported to C");

  chip = argcheck_fequals(ctx.ac, "--chip", "-c", "Chip to emulate [ h1b3c | h1c3m | h1d3c | h1d3m | npcx ]");
  ctx.verbose = argcheck_fbool(ctx.ac, "--verbose", "-v", "Verbose output");

  argcheck_check_help(ctx.ac);

  /* The default chip is H1B3C. */
  if (!chip) chip = "h1b3c";

  if (strcmp(chip, "h1b3c") == 0) {
    ctx.chip = H1B3C;
  } else {
    LOG_ERR("Invalid chip passed!\n");
    emu_exit(RC_ARG_CHIP_INVALID);
  }

  get_chipemu(ctx.chip);

  ctx.chipemu.initialize();
  if (!ctx.chipemu.ready) emu_exit(RC_CHIPEMU_FAILED_INIT);

  ctx.chipemu.start_emulation();

  /* Emulation is done, end it. */
  ctx.chipemu.end_emulation();

  argcheck_destroy(ctx.ac);
  ctx.ac = NULL;
  return RC_SUCCESS;
}
