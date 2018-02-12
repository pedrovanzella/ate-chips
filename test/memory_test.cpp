#include "memory.h"
#include "rom.h"
#include "gtest/gtest.h"
#include <vector>

class MemoryTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::vector<uint8_t> buffer;

    for (int i = 0; i <= 0x400; ++i) {
      buffer.push_back(0xab);
    }

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

TEST_F(MemoryTest, ShouldReadFromRAM) { EXPECT_EQ(_memory[0x601], 0x00); }
