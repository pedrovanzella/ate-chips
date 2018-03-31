#include "cpu.h"
#include "rom.h"
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

TEST_F(CPUTest, MOVA_$VX) {
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
