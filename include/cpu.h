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
  uint16_t SP;

  bool keypad[16];

  void push_to_stack(uint16_t val);
  uint16_t pop_from_stack();

  // Timers count down from 0xff to 0x0 at 60hz
  uint8_t delay_timer;
  uint8_t sound_timer;

  void start_timers();

private:
  Memory _memory;
};
} // namespace atechips
