#include "atechips.h"
#include "gtest/gtest.h"

TEST(SanityCheckTest, Works) { EXPECT_EQ(1, 1); }

TEST(SanityCheckTest, IncludesLib) {
  EXPECT_EQ(atechips::sanity(), "I ate 8 chips\n");
}
