#include "memory.h"
#include "rom.h"
#include "gtest/gtest.h"
#include <vector>

class MemoryTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::array<uint8_t, 1024> buffer;
    buffer.fill(0xab);

    auto rom = atechips::ROM(buffer);
    _memory.loadROM(rom);
  }

  atechips::Memory _memory;
};

TEST_F(MemoryTest, ShouldReadFromROM) {
  EXPECT_EQ(_memory[0x200], 0xabab);
  EXPECT_EQ(_memory[0x300], 0xabab);
  EXPECT_EQ(_memory[0x400], 0xabab);
  EXPECT_EQ(_memory[0x500], 0xabab);
  EXPECT_EQ(_memory[0x5FE], 0xabab);
}

TEST_F(MemoryTest, ShouldReadFromRAM) {
  EXPECT_EQ(_memory[0x600], 0x00);
  EXPECT_EQ(_memory[0x700], 0x00);
  EXPECT_EQ(_memory[0x800], 0x00);
  EXPECT_EQ(_memory[0x900], 0x00);
  EXPECT_EQ(_memory[0xA00], 0x00);
}

TEST_F(MemoryTest, ShouldWriteToRAM) {
  _memory.write(0x642, 0xabcd);
  EXPECT_EQ(_memory[0x642], 0xabcd);
}

TEST_F(MemoryTest, ShouldNotWriteToROM) {
  _memory.write(0x242, 0xffff);
  EXPECT_EQ(_memory[0x242], 0xabab);
}
