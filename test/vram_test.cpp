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
    
}

TEST_F(VramTest, ShouldWriteByteOutOfBounds) {

}

TEST_F(VramTest, ShouldFlagCollision) {

}