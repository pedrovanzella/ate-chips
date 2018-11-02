#pragma once

#include "atechips.h"
#include "rom.h"
#include <array>
#include <cstdint>

namespace atechips {

typedef bool uint1_t;
class Memory {
public:
  Memory();
  void loadROM(ROM rom);

  uint16_t operator[](uint16_t addr);
  void write(uint16_t addr, uint16_t val);
  void write_byte(uint16_t addr, uint8_t val);
  uint8_t get_byte(uint16_t addr);

  uint1_t get_video_bit(uint8_t addr);
  void write_video_bit(uint8_t addr, uint1_t bit);

private:
  ROM _rom;
  std::array<uint8_t, 2560> _ram;
};
} // namespace atechips
