#pragma once

#include "atechips.h"
#include "rom.h"
#include "vram.h"
#include <array>
#include <cstdint>

namespace atechips {


class Memory {
public:
  Memory();
  void loadROM(ROM rom);

  uint16_t operator[](uint16_t addr);
  void write(uint16_t addr, uint16_t val);
  void write_byte(uint16_t addr, uint8_t val);
  uint8_t get_byte(uint16_t addr);

  Vram& vram();

private:
  ROM _rom;
  std::array<uint8_t, 2560> _ram;
  Vram _vram;
};
  
std::pair<uint8_t, uint8_t> addr_to_coords(uint16_t addr);
} // namespace atechips
