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
