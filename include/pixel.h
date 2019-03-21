#pragma once

#include "atechips.h"
#include <SFML/Graphics.hpp>

namespace atechips {

struct Pixel {
  static constexpr float SIZE = 50.f;
  sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(SIZE, SIZE));

  Pixel() { rect.setFillColor(sf::Color::Green); }

  void toggle();
  void move(int x, int y);

private:
  sf::Vector2f position = sf::Vector2f(0, 0);
};

} // namespace atechips
