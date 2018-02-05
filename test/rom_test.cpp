#include "gtest/gtest.h"
#include "rom.h"
#include <cstdint>
#include <vector>

class ROMTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::vector<uint8_t> buff = {0, 1, 2, 3, 4, 5};
    _simple_rom.setBuffer(buff);

    std::vector<uint8_t> cbuff = {0x00, 0xe0, // CLS
                                  0x00, 0xee, // RET
                                  0x12, 0x00, // JMP $200
                                  0x22, 0x00, // CALL $(200)
                                  0x30, 0x00, // SEQ V0 $00
                                  0x40, 0x00, // SNE V0 $00
                                  0x50, 0x10, // SEQ V0 V1
                                  0x60, 0xab, // MOV V0 $AB
                                  0x70, 0x02, // ADD V0 $02
                                  0x80, 0x10, // MOV V0 V1
                                  0x80, 0x11, // OR V0 V1
                                  0x80, 0x12, // AND V0 V1
                                  0x80, 0x13, // XOR V0 V1
                                  0x80, 0x14, // ADD V0 V1
                                  0x80, 0x15, // SUB V0 V1
                                  0x80, 0x16, // SHIFTR V0 V1
                                  0x80, 0x17, // DIFF V0 V1
                                  0x80, 0x1e, // SHIFTL V0 V1
                                  0x90, 0x10, // SNE V0 V1
                                  0xa2, 0x00, // MOVI $200
                                  0xb2, 0x00, // JMPO $200
                                  0xc0, 0xab, // RANDAND V0 $AB
                                  0xd0, 0x1f, // DRAW V0 V1 $F
                                  0xe0, 0x9e, // JEQK (V0)
                                  0xe0, 0xa1, // JNEQK (V0)
                                  0xf0, 0x07, // MOVT V0
                                  0xf0, 0x0a, // MOVK V0
                                  0xf0, 0x15, // MOVT $V0
                                  0xf0, 0x18, // MOVST $V0
                                  0xf0, 0x1e, // MOVI $V0
                                  0xf0, 0x29, // MOVI $(V0)
                                  0xf0, 0x33, // BCD V0
                                  0xfa, 0x55, // MOVA $VA
                                  0xfa, 0x65, // LDA $VA
    };
    _complex_rom.setBuffer(cbuff);
  }

  atechips::ROM _simple_rom;
  atechips::ROM _complex_rom;
};

TEST_F(ROMTest, ReadsBytes) {
  EXPECT_EQ(_simple_rom.get_byte(0), 0);
  EXPECT_EQ(_simple_rom.get_byte(5), 5);
}

TEST_F(ROMTest, ReadsWord) {
  EXPECT_EQ(_simple_rom.get_word(0), (0 << 8) + 1);
  EXPECT_EQ(_simple_rom.get_word(1), (2 << 8) + 3);
  EXPECT_EQ(_simple_rom.get_word(2), (4 << 8) + 5);
}

TEST_F(ROMTest, GetsNibblesRight) {
  EXPECT_NE(_simple_rom.disassemble_word(0), "SOMETHING IS WRONG!");
}

TEST_F(ROMTest, DisassemblesEveryOPCode) {
  
}

