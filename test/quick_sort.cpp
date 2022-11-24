
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>

#include <ranges>
#include <sorters/quick_sort.hpp>
#include <utils/array.hpp>
#include <utils/assets.hpp>

TEST(QuickSort, ShouldSortContentFromFile) {
  auto array =
      utils::Array::vector_from_file<uint32_t>("./assets/dec/d100.txt");

  sorters::QuickSort::sort(array);

  ASSERT_TRUE(std::ranges::is_sorted(array));
}

TEST(QuickSort, ShouldWorkForAllFiles) {
  for (auto const& operations : std::vector<std::string>{"asc", "dec", "random", "part_sorted"}) {
    auto vectors = utils::assets::get_vectors_from_assets(
        fmt::format("assets/{}", operations));

    for (auto& vector : vectors) {
      sorters::QuickSort::sort(vector);

      ASSERT_TRUE(std::ranges::is_sorted(vector));
    }
  }
}
