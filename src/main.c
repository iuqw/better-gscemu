#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <H1B3C/emulator.h>
#include <common/emulator.h>
#include <common/exit.h>
#include <common/logger.h>
#include <libargs.h>

Emulator get_chipemu(EmulatorChips chip, ArgCheck *ac) {
  Emulator chipemu = {0};

  switch (chip) {
    case H1B3C:
      chipemu.initialize = Havn_Initialize;
      chipemu.start_emulation = Havn_StartEmulation;
      chipemu.end_emulation = Havn_EndEmulation;
      return chipemu;
    default:
      emu_exit(-1, ac, NULL, NULL);
  }

  return chipemu;
}

int main(int argc, char **argv) {
  ArgCheck *ac = argcheck_new(argc, argv);
  if (!ac) return 1;

  argcheck_set_description(ac, "gscemulator ported to C");

  const char *chip = argcheck_fequals(ac, "--chip", "-c", "Chip to emulate [ h1b3c | h1c3m | h1d3c | h1d3m | npcx ]");
  int verbose = argcheck_fbool(ac, "--verbose", "-v", "Verbose output");
  if (verbose) isVerboseLogging = true;

  argcheck_check_help(ac);

  /* The default chip is H1B3C. */
  if (!chip) chip = "h1b3c";
  EmulatorChips chipType = H1B3C;

  if (strcmp(chip, "h1b3c") == 0) {
    chipType = H1B3C;
  } else {
    LOG_ERR("Invalid chip passed!\n");
    emu_exit(-1, ac, NULL, NULL);
  }

  Emulator chipemu = get_chipemu(chipType, ac);

  uc_engine *uc = chipemu.initialize(&chipemu, ac);
  if (!chipemu.ready) {
    emu_exit(0, ac, &chipemu, uc);
  }

  chipemu.start_emulation(uc);

  /* Emulation is done, end it. */
  chipemu.end_emulation(uc);

  argcheck_destroy(ac);
  return 0;
}
