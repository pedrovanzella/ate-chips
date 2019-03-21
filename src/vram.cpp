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
    auto column = col + 7;
    for (int i = 0; i < 8; ++i) {
        uint1_t bit = byte & (1 << i);
        if (!bit && read_bit(row, column)) {
            collision = true;
        }
        write_bit(row, column, bit);
        column -= 1;
    }
    return collision;
}

uint8_t Vram::read_byte(uint8_t row, uint8_t col) {
    uint8_t byte = 0;

    for (auto column = col; column < max_col && column < col + 7; ++column) {
        byte += _vram[row][column];
        byte <<= 1;
    }
    byte += _vram[row][col + 7];

    return byte;
}
