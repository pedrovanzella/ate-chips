#include "memory.h"
#include "rom.h"

using namespace atechips;

Memory::Memory() {
  _ram.fill(0);
  // Initialize font data
  std::array<uint8_t, 80> fonts = {0xf0, 0x90, 0x90, 0x90, 0xf0,
                                   0x20, 0x60, 0x20, 0x20, 0x70,
                                   0xf0, 0x10, 0xf0, 0x80, 0xf0,
                                   0xf0, 0x10, 0xf0, 0x10, 0xf0,
                                   0x90, 0x90, 0xf0, 0x10, 0x10,
                                   0xf0, 0x80, 0xf0, 0x10, 0xf0,
                                   0xf0, 0x80, 0xf0, 0x90, 0xf0,
                                   0xf0, 0x10, 0x20, 0x40, 0x40,
                                   0xf0, 0x90, 0xf0, 0x90, 0xf0,
                                   0xf0, 0x90, 0xf0, 0x10, 0xf0,
                                   0xf0, 0x90, 0xf0, 0x90, 0x90,
                                   0xe0, 0x90, 0xe0, 0x90, 0xe0,
                                   0xf0, 0x80, 0x80, 0x80, 0xf0,
                                   0xe0, 0x90, 0x90, 0x90, 0xe0,
                                   0xf0, 0x80, 0xf0, 0x80, 0xf0,
                                   0xf0, 0x80, 0xf0, 0x80, 0x80
  };
  for (size_t i = 0; i <= fonts.size(); i++) {
    _ram[i] = fonts[i];
  }
}

uint8_t Memory::get_byte(uint16_t addr) {
  return _ram[addr];
}

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
