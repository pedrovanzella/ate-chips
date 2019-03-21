#include "pixel.h"

using namespace atechips;

void Pixel::toggle() {
  if (rect.getFillColor() == sf::Color::Transparent) {
    rect.setFillColor(sf::Color::Green);
  }
  rect.setFillColor(sf::Color::Transparent);
}

void Pixel::move(int x, int y) {
  position += sf::Vector2f(x * SIZE, y * SIZE);
  rect.setPosition(position);
}
