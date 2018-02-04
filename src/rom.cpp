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
  return 0;
}

std::string ROM::disassemble_word(int offset)
{
  return "NOT IMPLEMENTED";
}
