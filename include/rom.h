#include <cstdint>
#include <vector>
#include <string>

namespace atechips {
  class ROM {
  public:
    ROM(std::vector<uint8_t> _buf);
    ROM();
    void setBuffer(std::vector<uint8_t> buff);
    uint8_t get_byte(int offset);
    uint16_t get_word(int offset);
    std::string disassemble_word(int offset);

  private:
    std::vector<uint8_t> _buffer;
    uint8_t nibbles_for_word(uint16_t word, uint8_t nib);
    std::string nibble_to_hex(uint8_t nibble);
  };
}
