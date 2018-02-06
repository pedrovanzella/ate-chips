#include <iterator>
#include <cstdio>
#include <string>
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
  uint8_t nibbles[4] = {nibbles_for_word(word, 3), // Opcodes are encoded in the first 4 bits
                        nibbles_for_word(word, 2),
                        nibbles_for_word(word, 1),
                        nibbles_for_word(word, 0),
                        };

  switch (nibbles[0]) {
  case 0x00:
    if (nibbles[3] == 0x0) {
      return "CLS";
    }
    if (nibbles[3] == 0xe) {
      return "RET";
    }
    return "NOT IMPLEMENTED";
  case 0x01:
    return "JMP $" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x02:
    return "CALL $(" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]) + ')';
  case 0x03:
    return "SEQ V" + nibble_to_hex(nibbles[1]) + " $" + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x04:
    return "SNE V" + nibble_to_hex(nibbles[1]) + " $" + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x05:
    return "SEQ V" + nibble_to_hex(nibbles[1]) + " V" + nibble_to_hex(nibbles[2]);
  case 0x06:
    return "MOV V" + nibble_to_hex(nibbles[1]) + " $" + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x07:
    return "ADD V" + nibble_to_hex(nibbles[1]) + " $" + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x08:
    return "NOT IMPLEMENTED";
  case 0x09:
    return "SNE V" + nibble_to_hex(nibbles[1]) + " V" + nibble_to_hex(nibbles[2]);
  case 0x0a:
    return "MOVI $" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x0b:
    return "JMPO $" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x0c:
    return "RANDAND V" + nibble_to_hex(nibbles[1]) + " $" + nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x0d:
    return "DRAW V" + nibble_to_hex(nibbles[1]) + " V" + nibble_to_hex(nibbles[2]) + " $" + nibble_to_hex(nibbles[3]);
  case 0x0e:
    return "NOT IMPLEMENTED";
  case 0x0f:
    return "NOT IMPLEMENTED";
  default:
    return "SOMETHING IS WRONG!";
  }
}

uint8_t ROM::nibbles_for_word(uint16_t word, uint8_t nib)
{
  return (word >> (4*nib)) & 0x0F;
}

std::string ROM::nibble_to_hex(uint8_t nibble)
{
  char hex[2];

  std::sprintf(hex, "%X", nibble);

  return std::string(hex);
}
