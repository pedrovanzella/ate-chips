#include "atechips.h"
#include "rom.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

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
  istrm.seekg(0);

  std::vector<char> buffer;
  buffer.resize(size);

  istrm.read(&buffer[0], size);

  std::vector<uint8_t> new_buff(buffer.begin(), buffer.end());

  auto rom = ROM(new_buff);

  for (size_t i = 0; i <= rom.size(); ++i) {
    std::cout << 200 + i * 2 << '\t' << rom.get_hex_word(i) << '\t'
              << rom.disassemble_word(i) << '\n';
  }
}
