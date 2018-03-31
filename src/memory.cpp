#include "memory.h"
#include "rom.h"

using namespace atechips;

Memory::Memory() { _ram.fill(0); }

void Memory::loadROM(ROM rom) { _rom = std::move(rom); }

uint16_t Memory::operator[](uint16_t addr) {
  if (addr >= 0x200 && addr <= 0x600) {
    return _rom[addr - 0x200];
  }
  return (_ram[addr] << 8) + _ram[addr + 1];
}

void Memory::write(uint16_t addr, uint16_t val) {
  // Silently write to RAM even if the address is shadowed by ROM
  _ram[addr] = val >> 8;
  _ram[addr + 1] = val & 0xff;
}

void Memory::write_byte(uint16_t addr, uint8_t val) {
  _ram[addr] = val;
}
