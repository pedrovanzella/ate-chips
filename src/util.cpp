#include "util.h"

uint8_t atechips::util::nibbles_for_word(uint16_t word, uint8_t nib) {
  return (word >> (4 * nib)) & 0x0F;
}

std::string atechips::util::nibble_to_hex(uint8_t nibble) {
  char hex[2];

  std::sprintf(hex, "%X", nibble);

  return std::string(hex);
}
