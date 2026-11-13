#include <stdlib.h>
#include <stdint.h>

#include <libargs.h>
#include <common/emulator.h>

ARGCHECK_NORETURN void emu_exit(uint32_t rc, ArgCheck *ac, Emulator *chipemu, uc_engine *uc) {
  if (chipemu != NULL && chipemu->end_emulation != NULL) {
    chipemu->end_emulation(uc); 
  }
    
  argcheck_destroy(ac);
  exit(rc); 
}