#pragma once

#include <fmt/core.h>

#include <algorithm>
#include <fstream>
#include <random>
#include <ranges>
#include <vector>

namespace utils {
  class Array {
   private:
    std::random_device _rd;
    std::mt19937 _gen;
    std::uniform_int_distribution<> _dis;

   public:
    Array() : _rd(), _gen(_rd()) {}
    ~Array() = default;

    // Generate a random array of size N.
    template <typename T>
    std::vector<T> generate(int32_t N) {
      _dis = std::uniform_int_distribution<>(0, N);
      auto array = std::vector<T>(N);
      std::ranges::generate(array, [this]() { return _dis(_gen); });
      return array;
    }

    // Create a vector from a file
    template <typename T>
    static std::vector<T> vector_from_file(const std::string& file_path) {
      std::ifstream stream{file_path};

      if (!stream.is_open()) {
        throw std::runtime_error(
            fmt::format("Could not open file: {}", file_path));
      }

      return std::vector<T>(std::istream_iterator<T>(stream), {});
    }
  };
}  // namespace utils
