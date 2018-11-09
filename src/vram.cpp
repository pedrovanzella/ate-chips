#include "vram.h"

using namespace atechips;

Vram::Vram() {
    _vram = {{{0}, {0}}};
}

void Vram::write_bit(uint8_t row, uint8_t col, uint1_t bit) {
    if (row < max_row && col < max_col) {
        _vram[row][col] = bit;
    }
}

uint1_t Vram::read_bit(uint8_t row, u_int8_t col) {
    if (row < max_row && col < max_col) {
        return _vram[row][col];
    }
    return 0;
}

bool Vram::write_byte(uint8_t row, uint8_t col, uint8_t byte) {
    // return true if a bit is flipped from set to unset only
    bool collision = false;
    auto limit = col + 8;
    if (limit > max_col) limit = max_col;
    for (int i = col; i < limit; ++i) {
        auto bit = byte >> i & 1;
        if (!bit && read_bit(row, i)) collision = true;
        write_bit(row, i, bit);
    }
    return collision; // stub
}