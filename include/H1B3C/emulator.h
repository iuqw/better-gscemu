#ifndef H1B3C_EMULATOR_H
#define H1B3C_EMULATOR_H

#include <libargs.h>
#include <common/emulator.h>

void Havn_Initialize(Emulator *chipemu, ArgCheck *ac);
void Havn_StartEmulation(void);
void Havn_EndEmulation(void);

#endif
