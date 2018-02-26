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
  const uint8_t get_byte(const uint16_t offset) const;
  const uint16_t get_word(const uint16_t offset) const;
  const std::string get_hex_word(const uint16_t offset) const;
  const std::string disassemble_word(const uint16_t offset) const;
  size_t size();

  const uint16_t operator[](const uint16_t offset) const;

private:
  std::array<uint8_t, 1024> _buffer;
};
} // namespace atechips
