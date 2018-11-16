#include "atechips.h"
#include "rom.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <SFML/Graphics.hpp>

using namespace atechips;

void renderingThread(sf::RenderWindow* window) {
  std::cout << "[render]\n";

  window->setActive(true);

  while (window->isOpen()) {
    window->clear(sf::Color::Black);
    // draw

    // end the current frame
    window->display();
  }
}

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

  istrm.read(buffer.data(), size);

  std::array<uint8_t, 1024> new_buff {0};

  std::copy_n(std::make_move_iterator(buffer.begin()), size, new_buff.begin());

  auto rom = ROM(new_buff);

  for (size_t i = 0; i <= rom.size(); i += 2) {
    std::cout << std::hex << 0x200 + i << std::dec << '\t'
              << rom.get_hex_word(i) << '\t' << rom.disassemble_word(i) << '\n';
  }

  sf::RenderWindow window(sf::VideoMode(800, 600), "AteChips");
  window.setActive(false);

  std::thread render(renderingThread, &window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }

  render.join();

  return 0;
}