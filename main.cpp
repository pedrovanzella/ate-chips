#include <iostream>
#include <fstream>
#include <cstdint>
#include <memory>
#include <vector>
#include "atechips.h"
#include "rom.h"

using namespace atechips;

int main(int argc, char* argv[])
{
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

  std::vector<uint8_t> buffer;
  buffer.reserve(size);

  buffer.insert(buffer.begin(),
                std::istream_iterator<uint8_t>(istrm),
                std::istream_iterator<uint8_t>());

  auto rom = ROM(buffer);

  for (int i = 0; i <= rom.size(); ++i) {
    std::cout << 200 + i*2 << '\t' << rom.get_hex_word(i) << '\t' << rom.disassemble_word(i) << '\n';
  }
}
