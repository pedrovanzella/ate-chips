#include "cpu.h"
#include "rom.h"
#include "util.h"

using namespace atechips;

CPU::CPU() : V{0}, PC(0x200), I(0) {}

bool CPU::step() {
  /* Returns false if we hit an invalid instruction */
  auto op = fetch();
  uint8_t nibbles[4] = {
      util::nibbles_for_word(op,
                             3), // Opcodes are encoded in the first 4 bits
      util::nibbles_for_word(op, 2),
      util::nibbles_for_word(op, 1),
      util::nibbles_for_word(op, 0),
  };

  switch (nibbles[0]) {
  case 0x00:
    if (nibbles[2] != 0xe) {
      return false;
    }
    if (nibbles[3] == 0x0) {
      // CLS
      return true;
    }
    if (nibbles[3] == 0xe) {
      // RET
      return true;
    }
    return false;
  case 0x01:
    // JMP $NNN
    return true;
  case 0x02:
    // CALL $(NNN)
    return true;
  case 0x03:
    // SEQ VX $NN
    return true;
  case 0x04:
    // SNE VX $NN
    return true;
  case 0x05:
    // SEQ VX VY
    return true;
  case 0x06:
    // MOV VX $NN
    return true;
  case 0x07:
    // ADD VX $NN
    return true;
  case 0x08:
    switch (nibbles[3]) {
    case 0x00:
      // MOV VX VY
      return true;
    case 0x01:
      // OR VX VY
      return true;
    case 0x02:
      // AND VX VY
      return true;
    case 0x03:
      // XOR VX VY
      return true;
    case 0x04:
      // ADD VX VY
      return true;
    case 0x05:
      // SUB VX VY
      return true;
    case 0x06:
      // SHIFTR VX VY
      return true;
    case 0x07:
      // DIFF VX VY
      return true;
    case 0x0e:
      // SHIFTL VX VY
      return true;
    default:
      return false;
    }
  case 0x09:
    // SNE VX VY
    return true;
  case 0x0a:
    // MOVI $NNN
    return true;
  case 0x0b:
    // JMPO $NNN
    return true;
  case 0x0c:
    // RANDAND VX $AB
    return true;
  case 0x0d:
    // DRAW VX VY $N
    return true;
  case 0x0e:
    if (nibbles[2] == 0x09 && nibbles[3] == 0x0e) {
      // JEQK (VX)
      return true;
    }
    if (nibbles[2] == 0x0a && nibbles[3] == 0x01) {
      // JNEQK (VX)
      return true;
    }
    return false;
  case 0x0f:
    if (nibbles[2] == 0x00 && nibbles[3] == 0x07) {
      // MOVT VX
      return true;
    }
    if (nibbles[2] == 0x00 && nibbles[3] == 0x0a) {
      // MOVK VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x05) {
      // MOVT $VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x08) {
      // MOVST $VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x0e) {
      // MOVI $VX
      return true;
    }
    if (nibbles[2] == 0x02 && nibbles[3] == 0x09) {
      // MOVI $(VX)
      return true;
    }
    if (nibbles[2] == 0x03 && nibbles[3] == 0x03) {
      // BCD VX
      return true;
    }
    if (nibbles[2] == 0x05 && nibbles[3] == 0x05) {
      // MOVA $VX
      return true;
    }
    if (nibbles[2] == 0x06 && nibbles[3] == 0x05) {
      // LDA $VX
      return true;
    }
    return false;
  default:
    return false;
  }
  return false;
}

void CPU::loadROM(ROM rom) { _memory.loadROM(rom); }

uint16_t CPU::fetch() { return _memory[PC]; }
