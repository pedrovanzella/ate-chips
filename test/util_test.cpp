#include "util.h"
#include "gtest/gtest.h"

TEST(UtilTest, NibblesForWord) {
  EXPECT_EQ(atechips::util::nibbles_for_word(0xabcd, 3), 0xa);
  EXPECT_EQ(atechips::util::nibbles_for_word(0xabcd, 2), 0xb);
  EXPECT_EQ(atechips::util::nibbles_for_word(0xabcd, 1), 0xc);
  EXPECT_EQ(atechips::util::nibbles_for_word(0xabcd, 0), 0xd);
}

TEST(UtilTest, NibbleToHex) {
  EXPECT_EQ(atechips::util::nibble_to_hex(0x0), "0");
  EXPECT_EQ(atechips::util::nibble_to_hex(0xa), "A");
}
