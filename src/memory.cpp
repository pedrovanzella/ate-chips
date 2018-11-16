#include "memory.h"
#include "rom.h"

using namespace atechips;

Memory::Memory() : _vram() {
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
  if (addr >= vram_addr) {
    // read from vram
    auto [row, col] = addr_to_coords(addr);
    return _vram.read_byte(row, col);
  } else {
    return _ram[addr];
  }
}

void Memory::loadROM(ROM rom) { _rom = std::move(rom); }

uint16_t Memory::operator[](uint16_t addr) {
  if (addr >= rom_lower && addr <= rom_upper) {
    // read from ROM
    return _rom[addr - rom_lower];
  }
  return (get_byte(addr) << 8) + get_byte(addr + 1);
}

void Memory::write(uint16_t addr, uint16_t val) {
    write_byte(addr, val >> 8);
    write_byte(addr + 1, val & 0xff);
}

void Memory::write_byte(uint16_t addr, uint8_t val) {
  if (addr >= vram_addr) {
    // write to vram
    auto [row, col] = addr_to_coords(addr);
    _vram.write_byte(row, col, val);
  } else {
    // Silently write to RAM even if the address is shadowed by ROM
    _ram[addr] = val;
  }
}

Vram& Memory::vram() {
  return _vram;
}

std::pair<uint8_t, uint8_t> atechips::addr_to_coords(uint16_t addr) {
  auto base = (addr - atechips::Memory::vram_addr) * 8;
  auto row = base / Vram::max_col;
  auto col = base % Vram::max_col;
  return std::pair<uint8_t, uint8_t>(row, col);
}