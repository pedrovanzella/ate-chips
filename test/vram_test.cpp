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
    _vram.write_byte(20, 20, 0x0F);
    for (int i = 20; i < 24; ++i) {
        EXPECT_EQ(_vram.read_bit(20, i), 0);
    }
    for (int i = 24; i < 28; ++i) {
        EXPECT_EQ(_vram.read_bit(20, i), 1);
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

TEST_F(VramTest, ShouldReadByte) {
    _vram.write_bit(0, 0, 1);
    _vram.write_bit(0, 1, 0);
    _vram.write_bit(0, 2, 1);
    _vram.write_bit(0, 3, 0);
    _vram.write_bit(0, 4, 1);
    _vram.write_bit(0, 5, 0);
    _vram.write_bit(0, 6, 1);
    _vram.write_bit(0, 7, 0);
    EXPECT_EQ(_vram.read_byte(0, 0), 0b10101010);

    _vram.write_bit(1, 0, 1);
    _vram.write_bit(1, 1, 1);
    _vram.write_bit(1, 2, 1);
    _vram.write_bit(1, 3, 1);
    _vram.write_bit(1, 4, 1);
    _vram.write_bit(1, 5, 1);
    _vram.write_bit(1, 6, 1);
    _vram.write_bit(1, 7, 1);
    EXPECT_EQ(_vram.read_byte(1, 0), 0b11111111);
}
