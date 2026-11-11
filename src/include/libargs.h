#ifndef LIBARGS_H
#define LIBARGS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define ARGCHECK_NORETURN _Noreturn
#elif defined(__GNUC__) || defined(__clang__)
#define ARGCHECK_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define ARGCHECK_NORETURN __declspec(noreturn)
#else
#define ARGCHECK_NORETURN
#endif

typedef struct ArgCheck ArgCheck;

ArgCheck *argcheck_new(int argc, char **argv);
ArgCheck *argcheck_set_description(ArgCheck *ac, const char *desc);
void argcheck_destroy(ArgCheck *ac);

void argcheck_check_help(const ArgCheck *ac);
ARGCHECK_NORETURN void argcheck_show_help(const ArgCheck *ac);
const char *argcheck_fequals(ArgCheck *ac, const char *arg, const char *shorthand, const char *desc);
int argcheck_fbool(ArgCheck *ac, const char *arg, const char *shorthand, const char *desc);
const char *argcheck_fequals_str(ArgCheck *ac, const char *arg, const char *shorthand, const char *desc);

int argcheck_flag_count(const ArgCheck *ac);

#ifdef __cplusplus
}
#endif

#endif
