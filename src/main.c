/*
 *
 *   int main(int argc, char **argv)
 *   {
 *       ArgCheck *ac = argcheck_new(argc, argv);
 *       if (!ac) return 1;
 *
 *       argcheck_set_description(ac, "My tool – does things.");
 *
 *       const char *out = argcheck_fequals(ac, "--output", "-o", "Output file");
 *       int verbose     = argcheck_fbool  (ac, "--verbose", "-v", "Verbose output");
 *
 *       argcheck_destroy(ac);  // prints help and exits if --help / -h was given,
 *                              // then frees all resources
 *       ...
 *   }
*/

#include <stdio.h>

#include <libargs.h>

int main(int argc, char **argv){
  ArgCheck *ac = argcheck_new(argc, argv); if (!ac) return 1;
  argcheck_set_description(ac, "gscemulator ported to C");

  const char *out = argcheck_fequals(ac, "--chip", "-c", "Chip to emulate [ h1b2c | h1c3m | h1d3c | h1d3m | npcx ]");
  int verbose = argcheck_fbool(ac, "--verbose", "-v", "Verbose output");

  /* We're done with the args, free the resources. */
  argcheck_destroy(ac);

  printf("verbose: %d\n", verbose);
}