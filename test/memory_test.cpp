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
  EXPECT_EQ(_memory[0x601], 0x00);
  EXPECT_EQ(_memory[0x700], 0x00);
  EXPECT_EQ(_memory[0x800], 0x00);
  EXPECT_EQ(_memory[0x900], 0x00);
  EXPECT_EQ(_memory[0xA00], 0x00);
}

TEST_F(MemoryTest, ShouldWriteToRAM) {
  _memory.write(0x642, 0xabcd);
  EXPECT_EQ(_memory[0x642], 0xabcd);
}

TEST_F(MemoryTest, ShouldWriteByteToRAM) {
  _memory.write_byte(0x658, 0xab);
  _memory.write_byte(0x659, 0xcd);
  EXPECT_EQ(_memory[0x658], 0xabcd);
}

TEST_F(MemoryTest, ShouldNotWriteToROM) {
  _memory.write(0x242, 0xffff);
  EXPECT_EQ(_memory[0x242], 0xabab);
}

TEST_F(MemoryTest, ShouldWriteToVRAM) {
  _memory.write_byte(0xf00, 0xff);
  EXPECT_EQ(_memory.vram().read_byte(0, 0), 0xff);
  _memory.write_byte(0xf08, 0xab);
  EXPECT_EQ(_memory.vram().read_byte(1, 0), 0xab);
  _memory.write_byte(0xf13, 0xaa);
  EXPECT_EQ(_memory.vram().read_byte(2, 3 * 8), 0xaa);
}

TEST_F(MemoryTest, ShouldReadFromVRAM) {
  _memory.vram().write_byte(0, 0, 0xff);
  EXPECT_EQ(_memory.get_byte(0xf00), 0xff);
}

TEST(VideoCoordsTest, ShouldConvertFromAddrToCoords) {
  auto [row, col] = atechips::addr_to_coords(0xf00);
  EXPECT_EQ(row, 0);
  EXPECT_EQ(col, 0);

  auto [row2, col2] = atechips::addr_to_coords(0xf08);
  EXPECT_EQ(row2, 1);
  EXPECT_EQ(col2, 0);

  auto [row3, col3] = atechips::addr_to_coords(0xf0d);
  EXPECT_EQ(row3, 1);
  EXPECT_EQ(col3, (5 * 8));

  auto [row4, col4] = atechips::addr_to_coords(0xf11);
  EXPECT_EQ(row4, 2);
  EXPECT_EQ(col4, 8);
}