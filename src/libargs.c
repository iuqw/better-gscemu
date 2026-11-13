#include "libargs.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/rc.h>

typedef struct {
  char *long_name;
  char *short_name;
  char *description;
  int takes_value;
} FlagInfo;

struct ArgCheck {
  char **args;
  int argc;
  char *program_name;
  char *description;
  FlagInfo *flags;
  int flag_count;
  int flag_capacity;
  int help_requested;
};

static char *strdup_safe(const char *s) {
  size_t n;
  char *p;
  if (!s) return NULL;
  n = strlen(s);
  p = malloc(n + 1);
  if (!p) return NULL;
  return memcpy(p, s, n + 1);
}

static int flag_cmp(const void *a, const void *b) {
  const FlagInfo *fa = *(const FlagInfo *const *)a;
  const FlagInfo *fb = *(const FlagInfo *const *)b;
  if (!fa->long_name && !fb->long_name) return 0;
  if (!fa->long_name) return -1;
  if (!fb->long_name) return 1;
  return strcmp(fa->long_name, fb->long_name);
}

static void flag_info_clear(FlagInfo *fi) {
  if (!fi) return;
  free(fi->long_name);
  fi->long_name = NULL;
  free(fi->short_name);
  fi->short_name = NULL;
  free(fi->description);
  fi->description = NULL;
}

static void register_flag(ArgCheck *ac, const char *long_name, const char *short_name, const char *desc, int takes_value) {
  FlagInfo *fi;
  int i;

  if (!ac || !long_name || long_name[0] == '\0') return;

  for (i = 0; i < ac->flag_count; i++) {
    if (ac->flags[i].long_name &&
        strcmp(ac->flags[i].long_name, long_name) == 0)
      return;
  }

  if (ac->flag_count >= ac->flag_capacity) {
    int new_cap;
    FlagInfo *grown;

    if (ac->flag_capacity > INT_MAX / 2) return;
    new_cap = ac->flag_capacity * 2;

    if ((size_t)new_cap > SIZE_MAX / sizeof(FlagInfo)) return;

    grown = realloc(ac->flags, (size_t)new_cap * sizeof(FlagInfo));
    if (!grown) return;

    memset(grown + ac->flag_capacity, 0,
           (size_t)(new_cap - ac->flag_capacity) * sizeof(FlagInfo));

    ac->flags = grown;
    ac->flag_capacity = new_cap;
  }

  fi = &ac->flags[ac->flag_count];
  fi->long_name = strdup_safe(long_name);
  fi->short_name = strdup_safe(short_name ? short_name : "");
  fi->description = strdup_safe(desc ? desc : "");
  fi->takes_value = takes_value;

  if (!fi->long_name || !fi->short_name || !fi->description) {
    flag_info_clear(fi);
    return;
  }

  ac->flag_count++;
}

ArgCheck *argcheck_new(int argc, char **argv) {
  ArgCheck *ac;
  const char *prog;
  int i;

  if (argc < 0) argc = 0;

  ac = calloc(1, sizeof *ac);
  if (!ac) return NULL;

  ac->flag_capacity = 8;
  ac->flags = calloc(8, sizeof(FlagInfo));
  if (!ac->flags) {
    free(ac);
    return NULL;
  }

  ac->description = strdup_safe("");
  if (!ac->description) {
    free(ac->flags);
    free(ac);
    return NULL;
  }

  if (argc > 0 && argv) {
    ac->argc = argc;
    ac->args = calloc((size_t)argc, sizeof(char *));
    if (!ac->args) {
      free(ac->description);
      free(ac->flags);
      free(ac);
      return NULL;
    }
    for (i = 0; i < argc; i++) {
      ac->args[i] = strdup_safe(argv[i] ? argv[i] : "");
      if (!ac->args[i]) {
        int j;
        for (j = 0; j < i; j++) free(ac->args[j]);
        free(ac->args);
        free(ac->description);
        free(ac->flags);
        free(ac);
        return NULL;
      }
    }
  }

  prog = "program";
  if (argc > 0 && argv && argv[0] && argv[0][0]) {
    const char *sep = strrchr(argv[0], '/');
#ifdef _WIN32
    {
      const char *bs = strrchr(argv[0], '\\');
      if (!sep || (bs && bs > sep)) sep = bs;
    }
#endif
    {
      const char *base = sep ? sep + 1 : argv[0];
      if (*base) prog = base;
    }
  }
  ac->program_name = strdup_safe(prog);
  if (!ac->program_name) {
    for (i = 0; i < ac->argc; i++) free(ac->args[i]);
    free(ac->args);
    free(ac->description);
    free(ac->flags);
    free(ac);
    return NULL;
  }

  for (i = 0; i < ac->argc; i++) {
    const char *a = ac->args[i];
    if (a && (strcmp(a, "--help") == 0 || strcmp(a, "-h") == 0)) {
      ac->help_requested = 1;
      break;
    }
  }

  return ac;
}

ArgCheck *argcheck_set_description(ArgCheck *ac, const char *desc) {
  char *copy;
  if (!ac) return NULL;
  copy = strdup_safe(desc ? desc : "");
  if (!copy) return ac;
  free(ac->description);
  ac->description = copy;
  return ac;
}

void argcheck_check_help(const ArgCheck *ac) {
  if (ac && ac->help_requested) argcheck_show_help(ac);
}

ARGCHECK_NORETURN void argcheck_show_help(const ArgCheck *ac) {
  const char *prog = (ac && ac->program_name) ? ac->program_name : "program";

  printf("Usage: %s [OPTIONS]\n", prog);

  if (ac && ac->description && ac->description[0])
    printf("\n%s\n", ac->description);

  printf("\nOPTIONS:\n");

  if (!ac || ac->flag_count == 0) {
    printf("  %-20s %s\n", "-h, --help", "Show this help message");
    exit(RC_SUCCESS);
  }

  {
    const FlagInfo **sorted;
    size_t max_w;
    int col_w;
    int i;

    if ((size_t)ac->flag_count > SIZE_MAX / sizeof(FlagInfo *)) {
      printf("  %-20s %s\n", "-h, --help", "Show this help message");
      exit(RC_SUCCESS);
    }

    sorted = malloc((size_t)ac->flag_count * sizeof(FlagInfo *));
    if (!sorted) {
      printf("  %-20s %s\n", "-h, --help", "Show this help message");
      exit(RC_SUCCESS);
    }

    for (i = 0; i < ac->flag_count; i++) sorted[i] = &ac->flags[i];
    qsort(sorted, (size_t)ac->flag_count, sizeof(FlagInfo *), flag_cmp);

    max_w = 0;
    for (i = 0; i < ac->flag_count; i++) {
      const FlagInfo *fi = sorted[i];
      size_t slen, llen, vlen, w;
      if (!fi->long_name) continue;
      slen = (fi->short_name && fi->short_name[0])
                 ? strlen(fi->short_name) + 2 : 0;
      llen = strlen(fi->long_name);
      vlen = fi->takes_value ? 8u : 0u;
      w = slen + llen + vlen;
      if (w > max_w) max_w = w;
    }

    if (max_w > 1000u) max_w = 1000u;
    col_w = (int)(max_w + 10u);

    for (i = 0; i < ac->flag_count; i++) {
      const FlagInfo *fi = sorted[i];
      const char *vpart;
      size_t llen, slen, cap;
      char *fp;

      if (!fi->long_name) continue;

      vpart = fi->takes_value ? " <VALUE>" : "";
      llen = strlen(fi->long_name);
      slen = (fi->short_name && fi->short_name[0])
                 ? strlen(fi->short_name) : 0;

      cap = 4u + (slen ? slen + 2u : 0u) + llen + 8u + 1u;
      fp = malloc(cap);
      if (!fp) continue;

      if (slen > 0)
        snprintf(fp, cap, "%s, %s%s", fi->short_name, fi->long_name, vpart);
      else
        snprintf(fp, cap, "    %s%s", fi->long_name, vpart);

      printf("  %-*s %s\n", col_w, fp,
             fi->description ? fi->description : "");
      free(fp);
    }

    printf("  %-*s %s\n", col_w, "-h, --help", "Show this help message");
    free(sorted);
  }

  exit(RC_SUCCESS);
}

int argcheck_flag_count(const ArgCheck *ac) {
  return ac ? ac->flag_count : 0;
}

const char *argcheck_fequals(ArgCheck *ac, const char *arg, const char *shorthand, const char *desc) {
  size_t arg_len, sh_len;
  int i;

  if (!ac || !arg || arg[0] == '\0') return NULL;

  register_flag(ac, arg, shorthand ? shorthand : "", desc ? desc : "", 1);

  arg_len = strlen(arg);
  sh_len = (shorthand && shorthand[0]) ? strlen(shorthand) : 0;

  for (i = 0; i < ac->argc; i++) {
    const char *item = ac->args[i];
    if (!item) continue;

    if (strncmp(item, arg, arg_len) == 0 ||
        (sh_len > 0 && strncmp(item, shorthand, sh_len) == 0)) {

      const char *eq = strchr(item, '=');
      if (eq) {
        size_t plen = (size_t)(eq - item);
        if ((plen == arg_len &&
             strncmp(item, arg, plen) == 0) ||
            (sh_len > 0 && plen == sh_len &&
             strncmp(item, shorthand, plen) == 0)) {
          return eq + 1;
        }
      }
    }

    if (strcmp(item, arg) == 0 ||
        (sh_len > 0 && strcmp(item, shorthand) == 0)) {
      return (i + 1 < ac->argc) ? ac->args[i + 1] : NULL;
    }
  }

  return NULL;
}

int argcheck_fbool(ArgCheck *ac, const char *arg, const char *shorthand, const char *desc) {
  size_t sh_len;
  int i;

  if (!ac || !arg || arg[0] == '\0') return 0;

  register_flag(ac, arg, shorthand ? shorthand : "", desc ? desc : "", 0);

  sh_len = (shorthand && shorthand[0]) ? strlen(shorthand) : 0;

  for (i = 0; i < ac->argc; i++) {
    const char *item = ac->args[i];
    if (!item) continue;
    if (strcmp(item, arg) == 0 ||
        (sh_len > 0 && strcmp(item, shorthand) == 0))
      return 1;
  }

  return 0;
}

const char *argcheck_fequals_str(ArgCheck *ac, const char *arg, const char *shorthand, const char *desc) {
  const char *v = argcheck_fequals(ac, arg, shorthand, desc);
  return v ? v : "";
}

void argcheck_destroy(ArgCheck *ac) {
  int i;
  if (!ac) return;

  argcheck_check_help(ac);

  for (i = 0; i < ac->flag_count; i++) flag_info_clear(&ac->flags[i]);
  free(ac->flags);

  for (i = 0; i < ac->argc; i++) free(ac->args[i]);
  free(ac->args);

  free(ac->program_name);
  free(ac->description);
  free(ac);
}
