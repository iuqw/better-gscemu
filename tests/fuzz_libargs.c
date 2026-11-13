#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libargs.h>

#define MAX_ARGS 64

static int split_args(char *buf, char *argv[MAX_ARGS]) {
  int argc = 0;
  char *p = buf;

  while (*p != '\0' && argc < MAX_ARGS) {
    char *token = p;

    while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\n' &&
           *p != '\r' && *p != '\v' && *p != '\f') {
      p++;
    }

    if (*p != '\0') {
      *p = '\0';
      p++;
    }

    /* argcheck_destroy() prints help and exits on these. */
    if (strcmp(token, "--help") == 0 || strcmp(token, "-h") == 0) continue;

    argv[argc++] = token;
  }

  if (argc == 0) argv[argc++] = (char *)"gscemu";

  return argc;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  char *buf;
  char *argv[MAX_ARGS];
  int argc;
  const char *chip;
  const char *output;
  const char *name;
  int verbose;
  int debug;
  size_t sink = 0;

  buf = malloc(size + 1);
  if (!buf) return 0;

  memcpy(buf, data, size);
  buf[size] = '\0';

  for (size_t i = 0; i < size; i++) {
    if (buf[i] == '\0') buf[i] = ' ';
  }

  argc = split_args(buf, argv);

  ArgCheck *ac = argcheck_new(argc, argv);
  if (!ac) {
    free(buf);
    return 0;
  }

  argcheck_set_description(ac, "libargs fuzz target");

  chip = argcheck_fequals(ac, "--chip", "-c", "Chip");
  output = argcheck_fequals(ac, "--output", "-o", "Output");
  name = argcheck_fequals_str(ac, "--name", "-n", "Name");
  verbose = argcheck_fbool(ac, "--verbose", "-v", "Verbose");
  debug = argcheck_fbool(ac, "--debug", NULL, "Debug");

  if (chip) sink += strlen(chip);
  if (output) sink += strlen(output);
  if (name) sink += strlen(name);
  sink += (size_t)verbose + (size_t)debug;
  sink += (size_t)argcheck_flag_count(ac);
  (void)sink;

  argcheck_destroy(ac);
  free(buf);
  return 0;
}
