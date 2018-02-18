#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace atechips {
class ROM {
public:
  ROM(std::array<uint8_t, 1024> _buf);
  ROM();
  void setBuffer(std::array<uint8_t, 1024> buff);
  uint8_t get_byte(uint16_t offset);
  uint16_t get_word(uint16_t offset);
  std::string get_hex_word(uint16_t offset);
  std::string disassemble_word(uint16_t offset);
  size_t size();

  uint16_t operator[](uint16_t offset);

private:
  std::array<uint8_t, 1024> _buffer;
};
} // namespace atechips
