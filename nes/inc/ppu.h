#ifndef PPU_H
#define PPU_H

// 10KB of memory - 8KB on ROM and 2KB on machine.
// 2 more address spaces for a palette (colours) and OAM (position, orientation, shape and colour of sprites)
// Palette is static memory, OAM dynamic (and decays if PPU not rendering data)

// PPU exposes eight memory-mapped registers to CPU at $2000 - $2007, mirrored in every 8 bytes from $2008 to $3FFF
#endif
