#include "cpu.h"
#include "rom.h"

using namespace atechips;

CPU::CPU() : V{0}, PC(0x200), I(0) {}

bool CPU::step() {
  // Fetch
  // Decode
  // Execute
  return false;
}

void CPU::loadROM(ROM rom) { _memory.loadROM(rom); }

uint16_t CPU::fetch() { return _memory[PC]; }
