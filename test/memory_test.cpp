#include "memory.h"
#include "rom.h"
#include "gtest/gtest.h"
#include <vector>

class MemoryTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::vector<uint8_t> buffer;

    for (int i = 0; i <= 1024; ++i) {
      buffer.push_back(0xab);
    }

    auto rom = atechips::ROM(buffer);
    _memory.loadROM(rom);
  }

  atechips::Memory _memory;
};

TEST_F(MemoryTest, ShouldReadFromROM) {
  EXPECT_EQ(_memory[0x200], 0xabab);
  EXPECT_EQ(_memory[0x600], 0xabab);
}

TEST_F(MemoryTest, ShouldReadFromRAM) { EXPECT_EQ(_memory[0x601], 0x00); }
