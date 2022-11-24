#pragma once

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace sorters {
  class CountingSort {
   public:
    CountingSort() = delete;
    ~CountingSort() = delete;

    template <typename T>
    requires requires(T a) {
      std::is_integral<T>::value;
      std::is_unsigned<T>::value;
    }
    static std::vector<T> sort(std::vector<T> array) {
      auto max = *std::ranges::max_element(array);
      auto min = *std::ranges::min_element(array);
      auto count = std::vector<T>(max - min + 1, 0);

      for (auto const& element : array) count[element - min]++;

      for (auto i = 0, index = 0; i < count.size(); i++)
        while (count[i]-- > 0) array[index++] = i + min;

      return array;
    }
  };
}  // namespace sorters
