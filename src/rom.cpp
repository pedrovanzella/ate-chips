#include <iterator>
#include "rom.h"

using namespace atechips;

ROM::ROM(std::vector<uint8_t> buf) : _buffer(buf) {}

ROM::ROM() {}

void ROM::setBuffer(std::vector<uint8_t> buff)
{
  _buffer = buff;
}

uint8_t ROM::get_byte(int offset)
{
  return _buffer[offset];
}

uint16_t ROM::get_word(int offset)
{
  return (get_byte(offset * 2) << 8) + get_byte((offset * 2) + 1);
}

std::string ROM::disassemble_word(int offset)
{
  auto word = get_word(offset);
  uint8_t first_nibble = word >> 12; // Opcodes are encoded in the first 4 bits

  switch (first_nibble) {
  case 0x00:
  case 0x01:
  case 0x02:
  case 0x03:
  case 0x04:
  case 0x05:
  case 0x06:
  case 0x07:
  case 0x08:
  case 0x09:
  case 0x0a:
  case 0x0b:
  case 0x0c:
  case 0x0d:
  case 0x0e:
  case 0x0f:
    return "NOT IMPLEMENTED";
  default:
    return "SOMETHING IS WRONG!";
  }
}
