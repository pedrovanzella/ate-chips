#include "memory.h"
#include "rom.h"

using namespace atechips;

Memory::Memory() { _ram.fill(0); }

void Memory::loadROM(ROM rom) { _rom = rom; }

uint16_t Memory::operator[](uint16_t addr) {
  if (addr >= 0x200 && addr <= 0x600) {
    return _rom[addr - 0x200];
  }
  return _ram[addr];
}
