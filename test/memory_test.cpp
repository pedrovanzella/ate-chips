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
  for (int i = 0x200; i <= 0x600; ++i) {
    EXPECT_EQ(_memory[i], 0xabab);
  }
}

TEST_F(MemoryTest, ShouldReadFromRAM) { EXPECT_EQ(_memory[0x601], 0x00); }
