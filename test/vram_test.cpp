#include "vram.h"
#include "gtest/gtest.h"

class VramTest : public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    atechips::Vram _vram;
};

TEST_F(VramTest, ShouldBeInitializedToZero) {
    for (int row = 0; row < 32; ++row) {
        for (int col = 0; col < 64; ++col) {
            EXPECT_EQ(_vram.read_bit(row, col), 0);
        }
    }
}

TEST_F(VramTest, ShouldReadAndWriteToVram) {
    _vram.write_bit(2, 2, 1);
    EXPECT_EQ(_vram.read_bit(2, 1), 0);
    EXPECT_EQ(_vram.read_bit(2, 2), 1);
    EXPECT_EQ(_vram.read_bit(2, 3), 0);
    EXPECT_EQ(_vram.read_bit(1, 2), 0);
    EXPECT_EQ(_vram.read_bit(1, 3), 0);
}

TEST_F(VramTest, ShouldWriteByteWithinBounds) {
    _vram.write_byte(0, 0, 0b10101010);
    EXPECT_EQ(_vram.read_bit(0, 0), 1);
    EXPECT_EQ(_vram.read_bit(0, 1), 0);
    EXPECT_EQ(_vram.read_bit(0, 2), 1);
    EXPECT_EQ(_vram.read_bit(0, 3), 0);
    EXPECT_EQ(_vram.read_bit(0, 4), 1);
    EXPECT_EQ(_vram.read_bit(0, 5), 0);
    EXPECT_EQ(_vram.read_bit(0, 6), 1);
    EXPECT_EQ(_vram.read_bit(0, 7), 0);
}

TEST_F(VramTest, ShouldWriteByteOutOfBounds) {
    _vram.write_byte(0, 60, 0b11111111);
    EXPECT_EQ(_vram.read_bit(0, 60), 1);
    EXPECT_EQ(_vram.read_bit(0, 61), 1);
    EXPECT_EQ(_vram.read_bit(0, 62), 1);
    EXPECT_EQ(_vram.read_bit(0, 63), 1);
}

TEST_F(VramTest, ShouldFlagCollision) {
    _vram.write_bit(0, 0, 1);
    EXPECT_EQ(_vram.write_byte(0, 0, 0), true);
}