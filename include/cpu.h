#pragma once
#include "atechips.h"
#include "memory.h"
#include "rom.h"
#include <cstdint>

namespace atechips {
class CPU {
public:
  CPU();

  bool step();
  void loadROM(ROM rom);
  uint16_t fetch();
  uint16_t fetch(uint16_t addr);

  // useful for peripherals and test harnesses
  void write_to_mem(uint16_t addr, uint16_t val);

  uint8_t V[16];
  uint16_t PC;
  uint16_t I;

private:
  Memory _memory;
};
} // namespace atechips
