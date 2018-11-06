#pragma once

#include "atechips.h"
#include "rom.h"
#include <array>
#include <cstdint>
#include <bitset>

namespace atechips {


class Memory {
public:
  Memory();
  void loadROM(ROM rom);

  uint16_t operator[](uint16_t addr);
  void write(uint16_t addr, uint16_t val);
  void write_byte(uint16_t addr, uint8_t val);
  uint8_t get_byte(uint16_t addr);

private:
  ROM _rom;
  std::array<uint8_t, 2560> _ram;
};
} // namespace atechips
