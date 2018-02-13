#include "rom.h"
#include <cstdio>
#include <iterator>
#include <string>

using namespace atechips;

ROM::ROM(std::array<uint8_t, 1024> buf) { _buffer = std::move(buf); }

ROM::ROM() {}

void ROM::setBuffer(std::array<uint8_t, 1024> buff) {
  _buffer = std::move(buff);
}

uint8_t ROM::get_byte(uint16_t offset) { return _buffer[offset]; }

uint16_t ROM::get_word(uint16_t offset) {
  if (offset > _buffer.size()) {
    return 0x0;
  }
  return (get_byte(offset * 2) << 8) + get_byte((offset * 2) + 1);
}

uint16_t ROM::operator[](uint16_t offset) { return get_word(offset); }

std::string ROM::disassemble_word(uint16_t offset) {
  auto word = get_word(offset);
  uint8_t nibbles[4] = {
      nibbles_for_word(word, 3), // Opcodes are encoded in the first 4 bits
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
    return "JMP $" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) +
           nibble_to_hex(nibbles[3]);
  case 0x02:
    return "CALL $(" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) +
           nibble_to_hex(nibbles[3]) + ')';
  case 0x03:
    return "SEQ V" + nibble_to_hex(nibbles[1]) + " $" +
           nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x04:
    return "SNE V" + nibble_to_hex(nibbles[1]) + " $" +
           nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x05:
    return "SEQ V" + nibble_to_hex(nibbles[1]) + " V" +
           nibble_to_hex(nibbles[2]);
  case 0x06:
    return "MOV V" + nibble_to_hex(nibbles[1]) + " $" +
           nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x07:
    return "ADD V" + nibble_to_hex(nibbles[1]) + " $" +
           nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x08:
    switch (nibbles[3]) {
    case 0x00:
      return "MOV V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x01:
      return "OR V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x02:
      return "AND V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x03:
      return "XOR V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x04:
      return "ADD V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x05:
      return "SUB V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x06:
      return "SHIFTR V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x07:
      return "DIFF V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    case 0x0e:
      return "SHIFTL V" + nibble_to_hex(nibbles[1]) + " V" +
             nibble_to_hex(nibbles[2]);
    default:
      return "NOT IMPLEMENTED";
    }
  case 0x09:
    return "SNE V" + nibble_to_hex(nibbles[1]) + " V" +
           nibble_to_hex(nibbles[2]);
  case 0x0a:
    return "MOVI $" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) +
           nibble_to_hex(nibbles[3]);
  case 0x0b:
    return "JMPO $" + nibble_to_hex(nibbles[1]) + nibble_to_hex(nibbles[2]) +
           nibble_to_hex(nibbles[3]);
  case 0x0c:
    return "RANDAND V" + nibble_to_hex(nibbles[1]) + " $" +
           nibble_to_hex(nibbles[2]) + nibble_to_hex(nibbles[3]);
  case 0x0d:
    return "DRAW V" + nibble_to_hex(nibbles[1]) + " V" +
           nibble_to_hex(nibbles[2]) + " $" + nibble_to_hex(nibbles[3]);
  case 0x0e:
    if (nibbles[2] == 0x09 && nibbles[3] == 0x0e) {
      return "JEQK (V" + nibble_to_hex(nibbles[1]) + ")";
    }
    if (nibbles[2] == 0x0a && nibbles[3] == 0x01) {
      return "JNEQK (V" + nibble_to_hex(nibbles[1]) + ")";
    }
    return "NOT IMPLEMENTED";
  case 0x0f:
    if (nibbles[2] == 0x00 && nibbles[3] == 0x07) {
      return "MOVT V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x00 && nibbles[3] == 0x0a) {
      return "MOVK V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x05) {
      return "MOVT $V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x08) {
      return "MOVST $V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x0e) {
      return "MOVI $V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x02 && nibbles[3] == 0x09) {
      return "MOVI $(V" + nibble_to_hex(nibbles[1]) + ')';
    }
    if (nibbles[2] == 0x03 && nibbles[3] == 0x03) {
      return "BCD V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x05 && nibbles[3] == 0x05) {
      return "MOVA $V" + nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x06 && nibbles[3] == 0x05) {
      return "LDA $V" + nibble_to_hex(nibbles[1]);
    }
    return "NOT IMPLEMENTED";
  default:
    return "SOMETHING IS WRONG!";
  }
}

uint8_t ROM::nibbles_for_word(uint16_t word, uint8_t nib) {
  return (word >> (4 * nib)) & 0x0F;
}

std::string ROM::nibble_to_hex(uint8_t nibble) {
  char hex[2];

  std::sprintf(hex, "%X", nibble);

  return std::string(hex);
}

std::string ROM::get_hex_word(uint16_t offset) {
  auto first = get_byte(offset * 2);
  auto second = get_byte((offset * 2) + 1);
  char hex[5];
  std::sprintf(hex, "%02x%02x", first, second);
  return hex;
}

size_t ROM::size() { return _buffer.size(); }
