#pragma once
#include <cstdint>
#include <string>

namespace atechips {
namespace util {
uint8_t nibbles_for_word(uint16_t word, uint8_t nib);
std::string nibble_to_hex(uint8_t nibble);
} // namespace util
} // namespace atechips
