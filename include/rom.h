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
  };
}
