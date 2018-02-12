#pragma once

#include "atechips.h"
#include "rom.h"
#include <cstdint>

namespace atechips {
class Memory {
public:
  Memory();
  void loadROM(ROM rom);

  uint16_t operator[](uint16_t addr);

private:
  ROM _rom;
  uint8_t _ram[2560];
};
} // namespace atechips
