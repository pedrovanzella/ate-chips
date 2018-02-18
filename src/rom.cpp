#include "rom.h"
#include "util.h"
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
  return (get_byte(offset) << 8) + get_byte((offset) + 1);
}

uint16_t ROM::operator[](uint16_t offset) { return get_word(offset); }

std::string ROM::disassemble_word(uint16_t offset) {
  auto word = get_word(offset);
  uint8_t nibbles[4] = {
      util::nibbles_for_word(word,
                             3), // Opcodes are encoded in the first 4 bits
      util::nibbles_for_word(word, 2),
      util::nibbles_for_word(word, 1),
      util::nibbles_for_word(word, 0),
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
    return "JMP $" + util::nibble_to_hex(nibbles[1]) +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x02:
    return "CALL $(" + util::nibble_to_hex(nibbles[1]) +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]) +
           ')';
  case 0x03:
    return "SEQ V" + util::nibble_to_hex(nibbles[1]) + " $" +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x04:
    return "SNE V" + util::nibble_to_hex(nibbles[1]) + " $" +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x05:
    return "SEQ V" + util::nibble_to_hex(nibbles[1]) + " V" +
           util::nibble_to_hex(nibbles[2]);
  case 0x06:
    return "MOV V" + util::nibble_to_hex(nibbles[1]) + " $" +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x07:
    return "ADD V" + util::nibble_to_hex(nibbles[1]) + " $" +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x08:
    switch (nibbles[3]) {
    case 0x00:
      return "MOV V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x01:
      return "OR V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x02:
      return "AND V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x03:
      return "XOR V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x04:
      return "ADD V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x05:
      return "SUB V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x06:
      return "SHIFTR V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x07:
      return "DIFF V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    case 0x0e:
      return "SHIFTL V" + util::nibble_to_hex(nibbles[1]) + " V" +
             util::nibble_to_hex(nibbles[2]);
    default:
      return "NOT IMPLEMENTED";
    }
  case 0x09:
    return "SNE V" + util::nibble_to_hex(nibbles[1]) + " V" +
           util::nibble_to_hex(nibbles[2]);
  case 0x0a:
    return "MOVI $" + util::nibble_to_hex(nibbles[1]) +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x0b:
    return "JMPO $" + util::nibble_to_hex(nibbles[1]) +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x0c:
    return "RANDAND V" + util::nibble_to_hex(nibbles[1]) + " $" +
           util::nibble_to_hex(nibbles[2]) + util::nibble_to_hex(nibbles[3]);
  case 0x0d:
    return "DRAW V" + util::nibble_to_hex(nibbles[1]) + " V" +
           util::nibble_to_hex(nibbles[2]) + " $" +
           util::nibble_to_hex(nibbles[3]);
  case 0x0e:
    if (nibbles[2] == 0x09 && nibbles[3] == 0x0e) {
      return "JEQK (V" + util::nibble_to_hex(nibbles[1]) + ")";
    }
    if (nibbles[2] == 0x0a && nibbles[3] == 0x01) {
      return "JNEQK (V" + util::nibble_to_hex(nibbles[1]) + ")";
    }
    return "NOT IMPLEMENTED";
  case 0x0f:
    if (nibbles[2] == 0x00 && nibbles[3] == 0x07) {
      return "MOVT V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x00 && nibbles[3] == 0x0a) {
      return "MOVK V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x05) {
      return "MOVT $V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x08) {
      return "MOVST $V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x0e) {
      return "MOVI $V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x02 && nibbles[3] == 0x09) {
      return "MOVI $(V" + util::nibble_to_hex(nibbles[1]) + ')';
    }
    if (nibbles[2] == 0x03 && nibbles[3] == 0x03) {
      return "BCD V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x05 && nibbles[3] == 0x05) {
      return "MOVA $V" + util::nibble_to_hex(nibbles[1]);
    }
    if (nibbles[2] == 0x06 && nibbles[3] == 0x05) {
      return "LDA $V" + util::nibble_to_hex(nibbles[1]);
    }
    return "NOT IMPLEMENTED";
  default:
    return "SOMETHING IS WRONG!";
  }
}

std::string ROM::get_hex_word(uint16_t offset) {
  auto first = get_byte(offset);
  auto second = get_byte((offset) + 1);
  char hex[5];
  std::sprintf(hex, "%02x%02x", first, second);
  return hex;
}

size_t ROM::size() { return _buffer.size(); }
