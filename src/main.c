#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libargs.h>

int main(int argc, char **argv) {
  ArgCheck *ac = argcheck_new(argc, argv);
  if (!ac) return 1;

  argcheck_set_description(ac, "gscemulator ported to C");

  const char *chip = argcheck_fequals(ac, "--chip", "-c", "Chip to emulate [ h1b2c | h1c3m | h1d3c | h1d3m | npcx ]");
  int verbose = argcheck_fbool(ac, "--verbose", "-v", "Verbose output");

  argcheck_check_help(ac);

  // default chip is h1b2c
  if (!chip) chip = "h1b2c";

  argcheck_destroy(ac);
  return 0;
}