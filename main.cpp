#include "atechips.h"
#include "rom.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

using namespace atechips;

int main(int argc, char *argv[]) {
  std::cout << atechips::sanity();

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " rom\n";
    exit(1);
  }

  auto filename = argv[1];

  std::ifstream istrm(filename, std::ios::binary | std::ios::ate);
  if (!istrm.is_open()) {
    std::cerr << "Failed to open file " << filename << '\n';
    exit(1);
  }

  auto size = istrm.tellg();
  if (size > 1024) {
    std::cerr << "File size exceeds max ROM size of 1024\n";
    exit(1);
  }
  istrm.seekg(0);

  std::array<char, 1024> buffer;
  buffer.fill(0);

  istrm.read(&buffer[0], size);

  std::array<uint8_t, 1024> new_buff;

  std::copy_n(std::make_move_iterator(buffer.begin()), size, new_buff.begin());

  auto rom = ROM(new_buff);

  for (size_t i = 0; i <= rom.size(); ++i) {
    std::cout << std::hex << 200 + i * 2 << std::dec << '\t'
              << rom.get_hex_word(i) << '\t' << rom.disassemble_word(i) << '\n';
  }
}
