#include "gtest/gtest.h"
#include "rom.h"
#include <cstdint>
#include <vector>

class ROMTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::vector<uint8_t> buff = {0, 1, 2, 3, 4, 5};
    _simple_rom.setBuffer(buff);

    std::vector<uint8_t> cbuff = {};
    _complex_rom.setBuffer(cbuff);
  }

  atechips::ROM _simple_rom;
  atechips::ROM _complex_rom;
};

TEST_F(ROMTest, ReadsBytes) {
  EXPECT_EQ(_simple_rom.get_byte(0), 0);
  EXPECT_EQ(_simple_rom.get_byte(5), 5);
}

