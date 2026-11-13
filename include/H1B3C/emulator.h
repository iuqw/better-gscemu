#ifndef H1B3C_EMULATOR_H
#define H1B3C_EMULATOR_H

#include <unicorn/unicorn.h>

#include <libargs.h>
#include <common/emulator.h>

uc_engine *Havn_Initialize(Emulator *chipemu, ArgCheck *ac);
void Havn_StartEmulation(uc_engine *uc);
void Havn_EndEmulation(uc_engine *uc);

#endif
