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
      // TODO
      // CLS
      return true;
    }
    if (nibbles[3] == 0xe) {
      // TODO
      // RET
      return true;
    }
    return false;
  case 0x01:
    // JMP $NNN
    PC = (nibbles[1] << 8) + (nibbles[2] << 4) + (nibbles[3]);
    return true;
  case 0x02:
    // TODO
    // CALL $(NNN)
    return true;
  case 0x03:
    // TODO
    // SEQ VX $NN
    return true;
  case 0x04:
    // TODO
    // SNE VX $NN
    return true;
  case 0x05:
    // TODO
    // SEQ VX VY
    return true;
  case 0x06:
    // TODO
    // MOV VX $NN
    return true;
  case 0x07:
    // TODO
    // ADD VX $NN
    return true;
  case 0x08:
    switch (nibbles[3]) {
    case 0x00:
      // TODO
      // MOV VX VY
      return true;
    case 0x01:
      // TODO
      // OR VX VY
      return true;
    case 0x02:
      // TODO
      // AND VX VY
      return true;
    case 0x03:
      // TODO
      // XOR VX VY
      return true;
    case 0x04:
      // TODO
      // ADD VX VY
      return true;
    case 0x05:
      // TODO
      // SUB VX VY
      return true;
    case 0x06:
      // TODO
      // SHIFTR VX VY
      return true;
    case 0x07:
      // TODO
      // DIFF VX VY
      return true;
    case 0x0e:
      // TODO
      // SHIFTL VX VY
      return true;
    default:
      return false;
    }
  case 0x09:
    // TODO
    // SNE VX VY
    return true;
  case 0x0a:
    // TODO
    // MOVI $NNN
    return true;
  case 0x0b:
    // TODO
    // JMPO $NNN
    return true;
  case 0x0c:
    // TODO
    // RANDAND VX $AB
    return true;
  case 0x0d:
    // TODO
    // DRAW VX VY $N
    return true;
  case 0x0e:
    if (nibbles[2] == 0x09 && nibbles[3] == 0x0e) {
      // TODO
      // JEQK (VX)
      return true;
    }
    if (nibbles[2] == 0x0a && nibbles[3] == 0x01) {
      // TODO
      // JNEQK (VX)
      return true;
    }
    return false;
  case 0x0f:
    if (nibbles[2] == 0x00 && nibbles[3] == 0x07) {
      // TODO
      // MOVT VX
      return true;
    }
    if (nibbles[2] == 0x00 && nibbles[3] == 0x0a) {
      // TODO
      // MOVK VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x05) {
      // TODO
      // MOVT $VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x08) {
      // TODO
      // MOVST $VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x0e) {
      // TODO
      // MOVI $VX
      return true;
    }
    if (nibbles[2] == 0x02 && nibbles[3] == 0x09) {
      // TODO
      // MOVI $(VX)
      return true;
    }
    if (nibbles[2] == 0x03 && nibbles[3] == 0x03) {
      // TODO
      // BCD VX
      return true;
    }
    if (nibbles[2] == 0x05 && nibbles[3] == 0x05) {
      // TODO
      // MOVA $VX
      return true;
    }
    if (nibbles[2] == 0x06 && nibbles[3] == 0x05) {
      // TODO
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
