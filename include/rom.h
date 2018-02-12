#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace atechips {
class ROM {
public:
  ROM(std::vector<uint8_t> _buf);
  ROM();
  void setBuffer(std::vector<uint8_t> buff);
  uint8_t get_byte(uint16_t offset);
  uint16_t get_word(uint16_t offset);
  std::string get_hex_word(uint16_t offset);
  std::string disassemble_word(uint16_t offset);
  size_t size();

  uint16_t operator[](uint16_t offset);

private:
  std::vector<uint8_t> _buffer;
  uint8_t nibbles_for_word(uint16_t word, uint8_t nib);
  std::string nibble_to_hex(uint8_t nibble);
};
} // namespace atechips
