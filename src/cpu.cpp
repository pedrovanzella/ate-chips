#include "cpu.h"
#include "rom.h"
#include "util.h"
#include <cstdlib>

using namespace atechips;

CPU::CPU() : V{0}, PC(0x200), I(0), SP(0xea0), delay_timer(0xff), sound_timer(0xff) {}

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

  /* First we take care of our timers */
  /* Ideally this should be moved into a separate thread,
     to make sure they change at 60Hz. For now, though
     this is good enough */
  delay_timer--; // This is good enough to get them
  sound_timer--; // to wrap around

  switch (nibbles[0]) {
  case 0x00:
    if (nibbles[2] != 0xe) {
      return false;
    }
    if (nibbles[3] == 0x0) {
      // CLS
      for (int i = 0xf00; i <= 0xfff; i += 2) {
        _memory.write(i, 0x0000);
      }
      PC += 2;
      return true;
    }
    if (nibbles[3] == 0xe) {
      // RET
      PC = pop_from_stack();
      return true;
    }
    return false;
  case 0x01:
    // JMP $NNN
    PC = (nibbles[1] << 8) + (nibbles[2] << 4) + (nibbles[3]);
    return true;
  case 0x02:
    // CALL $(NNN)
    push_to_stack(PC + 2);
    PC = (nibbles[1] << 8) + (nibbles[2] << 4) + (nibbles[3]);
    return true;
  case 0x03:
    // SEQ VX $NN
    if (V[nibbles[1]] == ((nibbles[2] << 4) + (nibbles[3]))) {
      PC += 4;
    } else {
      PC += 2;
    }
    return true;
  case 0x04:
    // SNE VX $NN
    if (V[nibbles[1]] != ((nibbles[2] << 4) + (nibbles[3]))) {
      PC += 4;
    } else {
      PC += 2;
    }
    return true;
  case 0x05:
    // SEQ VX VY
    if (nibbles[3] != 0) {
      return false;
    }
    if (V[nibbles[1]] == V[nibbles[2]]) {
      PC += 4;
    } else {
      PC += 2;
    }
    return true;
  case 0x06:
    // MOV VX $NN
    V[nibbles[1]] = (nibbles[2] << 4) + (nibbles[3]);
    PC += 2;
    return true;
  case 0x07:
    // ADD VX $NN
    V[nibbles[1]] += (nibbles[2] << 4) + (nibbles[3]);
    PC += 2;
    return true;
  case 0x08:
    switch (nibbles[3]) {
    case 0x00:
      // MOV VX VY
      V[nibbles[1]] = V[nibbles[2]];
      PC += 2;
      return true;
    case 0x01:
      // OR VX VY
      V[nibbles[1]] |= V[nibbles[2]];
      PC += 2;
      return true;
    case 0x02:
      // AND VX VY
      V[nibbles[1]] &= V[nibbles[2]];
      PC += 2;
      return true;
    case 0x03:
      // XOR VX VY
      V[nibbles[1]] ^= V[nibbles[2]];
      PC += 2;
      return true;
    case 0x04:
      // ADD VX VY
      if (V[nibbles[1]] + V[nibbles[2]] > 0xff) {
        // Carry
        V[nibbles[1]] = V[nibbles[1]] + V[nibbles[2]] - 0xff;
        V[0xf] = 0x1;
      } else {
        V[nibbles[1]] += V[nibbles[2]];
      }
      PC += 2;
      return true;
    case 0x05:
      // SUB VX VY
      if (V[nibbles[1]] < V[nibbles[2]]) {
        // Borrow
        V[nibbles[1]] = V[nibbles[1]] - V[nibbles[2]] + 0xff;
        V[0xf] = 0x0;
      } else {
        V[nibbles[1]] -= V[nibbles[2]];
        V[0xf] = 0x1;
      }
      PC += 2;
      return true;
    case 0x06:
      // SHIFTR VX VY
      {
        auto lsb = V[nibbles[2]] >> 7;
        V[nibbles[1]] = V[nibbles[2]] = V[nibbles[2]] >> 1;
        V[0xf] = lsb;
      }
      PC += 2;
      return true;
    case 0x07:
      // DIFF VX VY
      if (V[nibbles[1]] > V[nibbles[2]]) {
        // Borrow
        V[nibbles[1]] = std::abs(V[nibbles[2]] - V[nibbles[1]]);
        V[0xf] = 0x0;
      } else {
        V[nibbles[1]] = V[nibbles[2]] - V[nibbles[1]];
        V[0xf] = 0x1;
      }
      PC += 2;
      return true;
    case 0x0e:
      // SHIFTL VX VY
      {
        auto msb = V[nibbles[2]] & 0x1;
        V[nibbles[1]] = V[nibbles[2]] = V[nibbles[2]] << 1;
        V[0xf] = msb;
      }
      PC += 2;
      return true;
    default:
      return false;
    }
  case 0x09:
    // SNE VX VY
    if (V[nibbles[1]] != V[nibbles[2]]) {
      PC += 4;
    } else {
      PC += 2;
    }
    return true;
  case 0x0a:
    // MOVI $NNN
    I = (nibbles[1] << 8) + (nibbles[2] << 4) + nibbles[3];
    PC += 2;
    return true;
  case 0x0b:
    // JMPO $NNN
    PC += V[0] + ((nibbles[1] << 8) + (nibbles[2] << 4) + nibbles[3]);
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
      // MOVT VX
      V[nibbles[1]] = delay_timer;
      PC += 2;
      return true;
    }
    if (nibbles[2] == 0x00 && nibbles[3] == 0x0a) {
      // TODO
      // MOVK VX
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x05) {
      // MOVT $VX
      delay_timer = V[nibbles[1]];
      PC += 2;
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x08) {
      // MOVST $VX
      sound_timer = V[nibbles[1]];
      PC += 2;
      return true;
    }
    if (nibbles[2] == 0x01 && nibbles[3] == 0x0e) {
      // MOVI $VX
      if (I + V[nibbles[1]] > 0xfff) {
        // Carry
        I = I + V[nibbles[1]] - 0xfff;
        V[0xf] = 0x01;
      } else {
        I += V[nibbles[1]];
        V[0xf] = 0x00;
      }
      PC += 2;
      return true;
    }
    if (nibbles[2] == 0x02 && nibbles[3] == 0x09) {
      // MOVI $(VX)
      I = V[nibbles[1]] * 5;
      PC += 2;
      return true;
    }
    if (nibbles[2] == 0x03 && nibbles[3] == 0x03) {
      // TODO
      // BCD VX
      return true;
    }
    if (nibbles[2] == 0x05 && nibbles[3] == 0x05) {
      // MOVA $VX
      for (int i = 0; i <= nibbles[1]; i++) {
        _memory.write_byte(I++, V[i]);
      }
      PC += 2;
      return true;
    }
    if (nibbles[2] == 0x06 && nibbles[3] == 0x05) {
      // LDA $VX
      for (int i = 0; i <= nibbles[1]; i++) {
        V[i] = _memory[I++];
      }
      PC += 2;
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

uint16_t CPU::fetch(uint16_t addr) { return _memory[addr]; }

void CPU::write_to_mem(uint16_t addr, uint16_t val) {
  _memory.write(addr, val);
}

void CPU::start_timers() {}

void CPU::push_to_stack(uint16_t val) {
  _memory.write(SP, val);
  SP += 2;
}

uint16_t CPU::pop_from_stack() {
  auto val = _memory[SP - 2];
  SP -= 2;
  return val;
}
