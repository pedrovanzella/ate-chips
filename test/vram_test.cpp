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
    _vram.write_byte(0, 0, 0xFF);
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(_vram.read_bit(0, i), 1);
    }
    _vram.write_byte(10, 10, 0xFF);
    for (int i = 10; i < 18; ++i) {
        EXPECT_EQ(_vram.read_bit(10, i), 1);
    }
}

TEST_F(VramTest, ShouldWriteByteOutOfBounds) {
    _vram.write_byte(0, 60, 0xFF);
    for (int i = 60; i < 64; ++i) {
        EXPECT_EQ(_vram.read_bit(0, i), 1);
    }
}

TEST_F(VramTest, ShouldFlagCollision) {
    _vram.write_bit(0, 0, 1);
    EXPECT_EQ(_vram.write_byte(0, 0, 0b01111111), true);
}