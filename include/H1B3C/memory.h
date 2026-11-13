#ifndef H1B3C_MEMORY_H
#define H1B3C_MEMORY_H

#define FLASH_ROM_BASE 0x00000000
#define FLASH_ROM_SIZE (8 * 1024) /* 8KiB */

#define SRAM_BASE 0x10000
#define SRAM_SIZE (64 * 1024) /* 64KiB */

/*
INFO is split into two regions:
- INFO0: 0x20000 (size: 0x800)
- INFO1: 0x28000 (size: 0x800)
*/
#define FLASH_INFO0_BASE 0x00020000
#define FLASH_INFO0_SIZE 0x800

#define FLASH_INFO1_BASE 0x00028000
#define FLASH_INFO1_SIZE 0x800

#define FLASH_PROG_BASE 0x40000
#define FLASH_PROG_SIZE (512 * 1024) /* 512KiB */

#endif /* H1B3C_MEMORY_H */