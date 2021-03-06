#include "cpu.h"
#include "rom.h"
#include "memory.h"
#include "gtest/gtest.h"

class CPUTest : public ::testing::Test {
protected:
  virtual void SetUp() {}

  atechips::CPU _cpu;
};

TEST_F(CPUTest, RegistersStartZeroed) {
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(_cpu.V[i], 0);
  }
  EXPECT_EQ(_cpu.I, 0);
}

TEST_F(CPUTest, PCStartsAt200) { EXPECT_EQ(_cpu.PC, 0x200); }

TEST_F(CPUTest, DoesntStepIfThereIsNoROMLoaded) {
  EXPECT_EQ(_cpu.step(), false);
}

TEST_F(CPUTest, LoadsROM) {
  auto rom = atechips::ROM({0xab, 0xab});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.fetch(), 0xabab);
}

TEST_F(CPUTest, JMP_NNN) {
  auto rom = atechips::ROM({0x13, 0x42});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x342);
}

TEST_F(CPUTest, SEQ_True) {
  auto rom = atechips::ROM({0x3a, 0xab});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xab;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x204);
}

TEST_F(CPUTest, SEQ_False) {
  auto rom = atechips::ROM({0x3a, 0xab});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, SNE_False) {
  auto rom = atechips::ROM({0x4a, 0xab});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xab;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, SNE_True) {
  auto rom = atechips::ROM({0x4a, 0xab});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x204);
}

TEST_F(CPUTest, SEQV_True) {
  auto rom = atechips::ROM({0x5a, 0xb0});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xab;
  _cpu.V[0xb] = 0xab;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x204);
}

TEST_F(CPUTest, SEQV_False) {
  auto rom = atechips::ROM({0x5a, 0xb0});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xff;
  _cpu.V[0xb] = 0xab;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, MOV_VX_NN) {
  auto rom = atechips::ROM({0x6a, 0x42});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.V[0xa], 0x42);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, ADD_VX_NN) {
  auto rom = atechips::ROM({0x7a, 0x05});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x2;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.V[0xa], 0x7);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, MOV_VX_VY) {
  auto rom = atechips::ROM({0x8a, 0xb0});
  _cpu.loadROM(rom);
  _cpu.V[0xb] = 0x42;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.V[0xa], 0x42);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, OR_VX_VY) {
  auto rom = atechips::ROM({0x8a, 0xb1});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xf0;
  _cpu.V[0xb] = 0x0f;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.V[0xa], 0xff);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, AND_VX_VY) {
  auto rom = atechips::ROM({0x8a, 0xb2});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xf0;
  _cpu.V[0xb] = 0x0f;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.V[0xa], 0xf0 & 0x0f);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, XOR_VX_VY) {
  auto rom = atechips::ROM({0x8a, 0xb3});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xf0;
  _cpu.V[0xb] = 0x0f;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.V[0xa], 0xf0 ^ 0x0f);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, SNE_VX_VY_True) {
  auto rom = atechips::ROM({0x9a, 0xb0});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xab;
  _cpu.V[0xb] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x204);
}

TEST_F(CPUTest, SNE_VX_VY_False) {
  auto rom = atechips::ROM({0x9a, 0xb0});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xff;
  _cpu.V[0xb] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, MOVI_NNN) {
  auto rom = atechips::ROM({0xa3, 0x42});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.I, 0x342);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, MOVA_dVX) {
  auto rom = atechips::ROM({0xff, 0x55});
  _cpu.loadROM(rom);
  _cpu.I = 0x602;
  for (int i = 0x0; i <= 0xf; i++) {
    _cpu.V[i] = 0xab;
  }
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  for (int i = 0x602; i <= 0x610; i += 2) {
    EXPECT_EQ(_cpu.fetch(i), 0xabab);
  }
  EXPECT_EQ(_cpu.I, 0x612);
}

TEST_F(CPUTest, LDA_dVX) {
  auto rom = atechips::ROM({0xff, 0x65});
  _cpu.loadROM(rom);
  _cpu.I = 0x602;
  for (int i = 0x602; i <= 0x612; i += 2) {
    _cpu.write_to_mem(i, 0xabcd);
  }
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  for (int i = 0x0; i <= 0xf; i++) {
    if (i % 2 == 1) {
      EXPECT_EQ(_cpu.V[i], 0xab);
    } else {
      EXPECT_EQ(_cpu.V[i], 0xcd);
    }
  }
  EXPECT_EQ(_cpu.I, 0x612);
}

TEST_F(CPUTest, CLS) {
  auto rom = atechips::ROM({0x00, 0xe0});
  _cpu.loadROM(rom);
  for (int i = atechips::Memory::vram_addr; i <= atechips::Memory::mem_limit; i += 2) {
    _cpu.write_to_mem(i, 0xffff);
  }
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  for (int i = atechips::Memory::vram_addr; i <= atechips::Memory::mem_limit; i+= 2) {
    EXPECT_EQ(_cpu.fetch(i), 0x0000);
  }
}

TEST_F(CPUTest, ADD_VX_VY_No_Carry) {
  auto rom = atechips::ROM({0x80, 0x14});
  _cpu.loadROM(rom);
  _cpu.V[0x0] = 0x01;
  _cpu.V[0x1] = 0x02;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0x0], 0x03);
  EXPECT_EQ(_cpu.V[0xf], 0x00);
}

TEST_F(CPUTest, ADD_VX_VY_With_Carry) {
  auto rom = atechips::ROM({0x80, 0x14});
  _cpu.loadROM(rom);
  _cpu.V[0x0] = 0xff;
  _cpu.V[0x1] = 0x03;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0x0], 0x03);
  EXPECT_EQ(_cpu.V[0xf], 0x01);
}

TEST_F(CPUTest, SUB_VX_VY_No_Borrow) {
  auto rom = atechips::ROM({0x80, 0x15});
  _cpu.loadROM(rom);
  _cpu.V[0x0] = 0x02;
  _cpu.V[0x1] = 0x01;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0x0], 0x01);
  EXPECT_EQ(_cpu.V[0xf], 0x01);
}

TEST_F(CPUTest, SUB_VX_VY_With_Borrow) {
  auto rom = atechips::ROM({0x80, 0x15});
  _cpu.loadROM(rom);
  _cpu.V[0x0] = 0x03;
  _cpu.V[0x1] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0x0], 0x03);
  EXPECT_EQ(_cpu.V[0xf], 0x00);
}

TEST_F(CPUTest, JMPO_dNNN) {
  auto rom = atechips::ROM({0xb2, 0x42});
  _cpu.loadROM(rom);
  _cpu.V[0] = 0x2;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x444);
}

TEST_F(CPUTest, MOVI_dVX_No_Carry) {
  auto rom = atechips::ROM({0xfa, 0x1e});
  _cpu.loadROM(rom);
  _cpu.I = 0x600;
  _cpu.V[0xf] = 0xff;
  _cpu.V[0xa] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.I, 0x6ff);
  EXPECT_EQ(_cpu.V[0xf], 0x0);
}

TEST_F(CPUTest, MOVI_dVX_With_Carry) {
  auto rom = atechips::ROM({0xfa, 0x1e});
  _cpu.loadROM(rom);
  _cpu.I = 0xf01;
  _cpu.V[0xf] = 0xff;
  _cpu.V[0xa] = 0xff;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.I, 0x001);
  EXPECT_EQ(_cpu.V[0xf], 0x1);
}

TEST_F(CPUTest, SHIFTR_VX_VY) {
  auto rom = atechips::ROM({0x8a, 0xb6});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xff;
  _cpu.V[0xb] = 0xab;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0xa], 0x55);
  EXPECT_EQ(_cpu.V[0xb], 0x55);
  EXPECT_EQ(_cpu.V[0xf], 0x1);
}

TEST_F(CPUTest, SHIFTL_VX_VY) {
  auto rom = atechips::ROM({0x8a, 0xbe});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0xff;
  _cpu.V[0xb] = 0xab;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0xa], 0x56);
  EXPECT_EQ(_cpu.V[0xb], 0x56);
  EXPECT_EQ(_cpu.V[0xf], 0x1);
}

TEST_F(CPUTest, DIFF_VX_VY_No_Borrow) {
  auto rom = atechips::ROM({0x80, 0x17});
  _cpu.loadROM(rom);
  _cpu.V[0x0] = 0x01;
  _cpu.V[0x1] = 0x02;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0x0], 0x01);
  EXPECT_EQ(_cpu.V[0xf], 0x01);
}

TEST_F(CPUTest, DIFF_VX_VY_With_Borrow) {
  auto rom = atechips::ROM({0x80, 0x17});
  _cpu.loadROM(rom);
  _cpu.V[0x0] = 0x04;
  _cpu.V[0x1] = 0x03;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0x0], 0x01);
  EXPECT_EQ(_cpu.V[0xf], 0x00);
}

TEST_F(CPUTest, MOVT_VX) {
  auto rom = atechips::ROM({0xfa, 0x07});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0xa], 0xfe);
}

TEST_F(CPUTest, MOVT_dVX) {
  auto rom = atechips::ROM({0xfa, 0x15});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.delay_timer, 0x00);
}

TEST_F(CPUTest, MOVST_dVX) {
  auto rom = atechips::ROM({0xfa, 0x18});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.sound_timer, 0x00);
}

TEST_F(CPUTest, CALL_dNNN) {
  auto rom = atechips::ROM({0x23, 0x00});
  _cpu.loadROM(rom);
  EXPECT_EQ(_cpu.PC, 0x200);
  EXPECT_EQ(_cpu.SP, 0xea0);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x300);
  EXPECT_EQ(_cpu.SP, 0xea2);
  EXPECT_EQ(_cpu.pop_from_stack(), 0x202);
}

TEST_F(CPUTest, RET) {
  auto rom = atechips::ROM({0x00, 0xee});
  _cpu.loadROM(rom);
  _cpu.push_to_stack(0x222);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.SP, 0xea0);
  EXPECT_EQ(_cpu.PC, 0x222);
}

TEST_F(CPUTest, MOVI_dVX) {
  auto rom = atechips::ROM({0xfa, 0x29});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x5;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.I, 0x19);
}

TEST_F(CPUTest, BCD_VX) {
  auto rom = atechips::ROM({0xfa, 0x33});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 123;
  _cpu.I = 0x700;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.fetch(0x700), (0x1 << 8) + 0x2);
  EXPECT_EQ(_cpu.fetch(0x702), 0x3 << 8);
}

TEST_F(CPUTest, JEQK_VX_True) {
  auto rom = atechips::ROM({0xea, 0x9e});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x5;
  _cpu.keypad[0x5] = true;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x204);
}

TEST_F(CPUTest, JEQK_VX_False) {
  auto rom = atechips::ROM({0xea, 0x9e});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x5;
  _cpu.keypad[0x5] = false;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, JNEQK_VX_True) {
  auto rom = atechips::ROM({0xea, 0xa1});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x5;
  _cpu.keypad[0x5] = false;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x204);
}

TEST_F(CPUTest, JNEQK_VX_False) {
  auto rom = atechips::ROM({0xea, 0xa1});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x5;
  _cpu.keypad[0x5] = true;
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
}

TEST_F(CPUTest, RANDAND_VX_dAB) {
  auto rom = atechips::ROM({0xca, 0xab});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0x42;
  _cpu.set_rng(100, 100);
  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.V[0xa], 0xab & 100);
}

TEST_F(CPUTest, DRAW_VX_VY_dN_No_collision) {
  auto rom = atechips::ROM({0xda, 0xb8});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0;
  _cpu.V[0xb] = 0;
  _cpu.I = 0x700;

  EXPECT_EQ(_cpu.fetch(atechips::Memory::vram_addr), 0x0); // vram starts blank

  for (auto a = 0x700; a < 0x70a; a += 2) {
    // Seed some arbitrary data where I points to
    _cpu.write_to_mem(a, 0xabab);
  }

  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.I, 0x700); // I doesn't change
  EXPECT_EQ(_cpu.V[0xf], 0x0); // collision is unset

  // Expect it to have written to the proper locations
  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ(_cpu.memory().vram().read_byte(i, 0), 0xab);
  }
}

TEST_F(CPUTest, DRAW_VX_VY_dN_With_collision) {
  auto rom = atechips::ROM({0xda, 0xb8});
  _cpu.loadROM(rom);
  _cpu.V[0xa] = 0;
  _cpu.V[0xb] = 0;
  _cpu.I = 0x700;

  EXPECT_EQ(_cpu.fetch(atechips::Memory::vram_addr), 0x0); // vram starts blank

  // write something to video memory
  _cpu.write_to_mem(atechips::Memory::vram_addr, 0xffff);

  EXPECT_EQ(_cpu.fetch(atechips::Memory::vram_addr), 0xffff); // wrote through MMU

  for (auto a = 0x700; a < 0x70a; a += 2) {
    // Seed some arbitrary data where I points to
    _cpu.write_to_mem(a, 0xabab);
  }

  EXPECT_EQ(_cpu.step(), true);
  EXPECT_EQ(_cpu.PC, 0x202);
  EXPECT_EQ(_cpu.I, 0x700);
  EXPECT_EQ(_cpu.V[0xf], 0x1); // collision should be set

  // Expect it to have written to the proper locations
  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ(_cpu.memory().vram().read_byte(i, 0), 0xab);
  }
}
