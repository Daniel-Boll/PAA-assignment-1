#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>

#include <ranges>
#include <sorters/counting_sort.hpp>
#include <utils/array.hpp>

TEST(CountingSort, ShouldSortContentFromFile) {
  auto array =
      utils::Array::vector_from_file<uint32_t>("./assets/asc/o100.txt");
  auto res = sorters::CountingSort::sort<uint32_t>(array);
  ASSERT_TRUE(std::ranges::is_sorted(res));
}
