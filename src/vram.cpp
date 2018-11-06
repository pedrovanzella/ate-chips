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
    auto collision = false;
    // return true if a bit is flipped from set to unset only
    if (col > max_col) return false;

    auto max = max_col - 8;
    if (max > max_col) max = max_col;
    for (int i = col; i < max; ++i) {
        if (_vram[col][i]) collision = true;
        _vram[row][i] = (byte >> i) & 1;
    }
    return collision;
}