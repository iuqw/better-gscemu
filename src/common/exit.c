#include <stdlib.h>
#include <stdint.h>

#include <libargs.h>
#include <common/emulator.h>

void emu_exit(uint32_t rc, ArgCheck *ac, Emulator *chipemu) {
  if (chipemu != NULL && chipemu->end_emulation != NULL) {
    chipemu->end_emulation(); 
  }
    
  argcheck_destroy(ac);
  exit(rc); 
}