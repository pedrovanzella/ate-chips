#include "rom.h"
#include "gtest/gtest.h"
#include <array>
#include <cstdint>

class ROMTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    std::array<uint8_t, 1024> buff{0, 1, 2, 3, 4, 5};
    _simple_rom.setBuffer(buff);

    std::array<uint8_t, 1024> cbuff{
        0x00, 0xe0, // CLS
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
  EXPECT_EQ(_simple_rom[0], (0 << 8) + 1);
  EXPECT_EQ(_simple_rom.get_word(2), (2 << 8) + 3);
  EXPECT_EQ(_simple_rom[2], (2 << 8) + 3);
  EXPECT_EQ(_simple_rom.get_word(4), (4 << 8) + 5);
  EXPECT_EQ(_simple_rom[4], (4 << 8) + 5);
}

TEST_F(ROMTest, GetsNibblesRight) {
  EXPECT_NE(_simple_rom.disassemble_word(0), "SOMETHING IS WRONG!");
}

TEST_F(ROMTest, DisassemblesEveryOPCode) {
  EXPECT_EQ(_complex_rom.disassemble_word(0), "CLS");
  EXPECT_EQ(_complex_rom.disassemble_word(2), "RET");
  EXPECT_EQ(_complex_rom.disassemble_word(4), "JMP $200");
  EXPECT_EQ(_complex_rom.disassemble_word(6), "CALL $(200)");
  EXPECT_EQ(_complex_rom.disassemble_word(8), "SEQ V0 $00");
  EXPECT_EQ(_complex_rom.disassemble_word(10), "SNE V0 $00");
  EXPECT_EQ(_complex_rom.disassemble_word(12), "SEQ V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(14), "MOV V0 $AB");
  EXPECT_EQ(_complex_rom.disassemble_word(16), "ADD V0 $02");
  EXPECT_EQ(_complex_rom.disassemble_word(18), "MOV V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(20), "OR V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(22), "AND V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(24), "XOR V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(26), "ADD V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(28), "SUB V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(30), "SHIFTR V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(32), "DIFF V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(34), "SHIFTL V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(36), "SNE V0 V1");
  EXPECT_EQ(_complex_rom.disassemble_word(38), "MOVI $200");
  EXPECT_EQ(_complex_rom.disassemble_word(40), "JMPO $200");
  EXPECT_EQ(_complex_rom.disassemble_word(42), "RANDAND V0 $AB");
  EXPECT_EQ(_complex_rom.disassemble_word(44), "DRAW V0 V1 $F");
  EXPECT_EQ(_complex_rom.disassemble_word(46), "JEQK (V0)");
  EXPECT_EQ(_complex_rom.disassemble_word(48), "JNEQK (V0)");
  EXPECT_EQ(_complex_rom.disassemble_word(50), "MOVT V0");
  EXPECT_EQ(_complex_rom.disassemble_word(52), "MOVK V0");
  EXPECT_EQ(_complex_rom.disassemble_word(54), "MOVT $V0");
  EXPECT_EQ(_complex_rom.disassemble_word(56), "MOVST $V0");
  EXPECT_EQ(_complex_rom.disassemble_word(58), "MOVI $V0");
  EXPECT_EQ(_complex_rom.disassemble_word(60), "MOVI $(V0)");
  EXPECT_EQ(_complex_rom.disassemble_word(62), "BCD V0");
  EXPECT_EQ(_complex_rom.disassemble_word(64), "MOVA $VA");
  EXPECT_EQ(_complex_rom.disassemble_word(66), "LDA $VA");
}

TEST_F(ROMTest, ReturnsProperHexRepresentationForWord) {
  EXPECT_EQ(_complex_rom.get_hex_word(0), "00e0");
  EXPECT_EQ(_complex_rom.get_hex_word(2), "00ee");
  EXPECT_EQ(_complex_rom.get_hex_word(4), "1200");
  EXPECT_EQ(_complex_rom.get_hex_word(6), "2200");
  EXPECT_EQ(_complex_rom.get_hex_word(8), "3000");
  EXPECT_EQ(_complex_rom.get_hex_word(10), "4000");
  EXPECT_EQ(_complex_rom.get_hex_word(12), "5010");
  EXPECT_EQ(_complex_rom.get_hex_word(14), "60ab");
  EXPECT_EQ(_complex_rom.get_hex_word(16), "7002");
  EXPECT_EQ(_complex_rom.get_hex_word(18), "8010");
  EXPECT_EQ(_complex_rom.get_hex_word(20), "8011");
  EXPECT_EQ(_complex_rom.get_hex_word(22), "8012");
  EXPECT_EQ(_complex_rom.get_hex_word(24), "8013");
  EXPECT_EQ(_complex_rom.get_hex_word(26), "8014");
  EXPECT_EQ(_complex_rom.get_hex_word(28), "8015");
  EXPECT_EQ(_complex_rom.get_hex_word(30), "8016");
  EXPECT_EQ(_complex_rom.get_hex_word(32), "8017");
  EXPECT_EQ(_complex_rom.get_hex_word(34), "801e");
  EXPECT_EQ(_complex_rom.get_hex_word(36), "9010");
  EXPECT_EQ(_complex_rom.get_hex_word(38), "a200");
  EXPECT_EQ(_complex_rom.get_hex_word(40), "b200");
  EXPECT_EQ(_complex_rom.get_hex_word(42), "c0ab");
  EXPECT_EQ(_complex_rom.get_hex_word(44), "d01f");
  EXPECT_EQ(_complex_rom.get_hex_word(46), "e09e");
  EXPECT_EQ(_complex_rom.get_hex_word(48), "e0a1");
  EXPECT_EQ(_complex_rom.get_hex_word(50), "f007");
  EXPECT_EQ(_complex_rom.get_hex_word(52), "f00a");
  EXPECT_EQ(_complex_rom.get_hex_word(54), "f015");
  EXPECT_EQ(_complex_rom.get_hex_word(56), "f018");
  EXPECT_EQ(_complex_rom.get_hex_word(58), "f01e");
  EXPECT_EQ(_complex_rom.get_hex_word(60), "f029");
  EXPECT_EQ(_complex_rom.get_hex_word(62), "f033");
  EXPECT_EQ(_complex_rom.get_hex_word(64), "fa55");
  EXPECT_EQ(_complex_rom.get_hex_word(66), "fa65");
}
