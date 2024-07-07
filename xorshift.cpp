#include <array>
#include <concepts>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

namespace impl {

class hashrand_generator {
public:
  template <std::integral T, std::integral... Rest>
  std::uint64_t operator()(T num, Rest... args) {
    _hash(num);
    return (*this)(args...);
  }

  std::uint64_t operator()() { return _state; }

private:
  template <std::integral T> void _hash(T num) {
    std::uint64_t mult = _multiplier();
    constexpr std::uint64_t half_bits = (sizeof(T) * 4);
    num = ((num >> half_bits) ^ num) * mult;
    num = ((num >> half_bits) ^ num) * mult;
    _state ^= (num >> half_bits) ^ num;
  }

  std::uint64_t _multiplier() {
    static std::array<std::uint64_t, 2> nums = {0x45d9f3b, 0xb3f9d54};
    return nums[++_counter % nums.size()];
  }

  std::uint64_t _state = 2463534242;
  std::uint64_t _counter = 0;
};

} // namespace impl

template <std::integral T, std::integral... Rest>
std::uint64_t hashrand(T num, Rest... args) {
  impl::hashrand_generator gen;
  return gen(num, args...);
}

int main() {
  constexpr std::uint32_t width = 10;
  constexpr std::uint32_t mod = 1000;

  std::cout << "n % " << mod << std::endl;
  std::cout << "   ";
  for (std::uint32_t i = 0; i < width; ++i) {
    std::cout << " | " << std::setw(3) << i;
  }
  std::cout << std::endl;

  for (std::uint32_t i = 0; i < width; ++i) {
    std::cout << std::setw(3) << i;
    for (std::uint32_t j = 0; j < width; ++j) {
      std::cout << " | " << std::setw(3) << (hashrand(i, j) % mod);
    }
    std::cout << std::endl;
  }

  return 0;
}
