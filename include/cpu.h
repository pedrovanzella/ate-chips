#pragma once
#include "atechips.h"

namespace atechips {
class CPU {
public:
  CPU();

  uint8_t V[16];
  uint16_t PC;
  uint16_t I;
};
} // namespace atechips
