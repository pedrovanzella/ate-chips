#pragma once
#include <array>
#include <cstdint>

namespace atechips {

typedef bool uint1_t;

class Vram {
public:
    static const int max_row = 32;
    static const int max_col = 64;
    Vram();

    void write_bit(uint8_t row, uint8_t col, uint1_t bit);
    uint1_t read_bit(uint8_t row, uint8_t col);

    // returns true if there was a collision
    bool write_byte(uint8_t row, uint8_t col, uint8_t byte);

    uint8_t read_byte(uint8_t row, uint8_t col);

private:
    // 32 rows of 64 cols
    std::array<std::array<uint1_t, max_col>, max_row> _vram;
};
}
