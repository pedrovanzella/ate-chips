#pragma once

#include "atechips.h"
#include "rom.h"
#include <array>
#include <cstdint>

namespace atechips {
class Memory {
public:
  Memory();
  void loadROM(ROM rom);

  uint16_t operator[](uint16_t addr);
  void write(uint16_t addr, uint16_t val);

private:
  ROM _rom;
  std::array<uint8_t, 2560> _ram;
};
} // namespace atechips
